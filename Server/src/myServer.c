#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/resource.h>


#include "Server.h"

#define GOODBYE "Peace out"
#define TCP_PORT 5100

volatile connection_t conn[50];
volatile int cnum = 0;
volatile pid_t pid;

// Set a file-descriptor to non-blocking mode
void setnonblocking(int fd) {
    int opts = fcntl(fd, F_GETFL);
    opts |= O_NONBLOCK;
    fcntl(fd, F_SETFL, opts);
}

static void sigHandler1(int signo) {
    P2P_MSG_t MSG;
    
    if (signo == SIGUSR1) {
        // This signal is received by the parent process
        // 1. Set the pipes of the connection(conn[i].pfd_CtoS) to non-blocking
        int i;
        for (i = 0; i < cnum; i++) {
            setnonblocking(conn[i].pfd_CtoS[0]);
        }

        // 2. Search through the pipes to find the right connection
        ssize_t bytesRead;
        for (i = 0; i < cnum; i++) {
            if ((bytesRead = read(conn[i].pfd_CtoS[0], &MSG, sizeof(P2P_MSG_t))) > 0) {
                break;
            }
        }

        // 3. Now MSG contains the right P2P_MSG. Send it to all connection pipes
        // Also, raise a signal for them
        for (i = 0; i < cnum; i++) {
	    MSG.cnum = i;
            write(conn[i].pfd_StoC[1], &MSG, sizeof(P2P_MSG_t));
            kill(conn[i].pid, SIGUSR2);
        }
    }
}

static void sigHandler2(int signo) {
    P2P_MSG_t MSG;

    if (signo == SIGUSR2) {
        // This signal is received by a child process, and used to BroadCast messages.
        // 1. Read a message from the server pipe (parent process)
        // 2. Write it to the connected client socket file descriptor (conn[cnum].sfd)
        ssize_t bytesRead = read(conn[cnum].pfd_StoC[0], &MSG, sizeof(P2P_MSG_t));
        if (bytesRead > 0) {
            send(conn[cnum].sfd, &MSG, sizeof(P2P_MSG_t), MSG_DONTWAIT);
        //} else {
        //    perror("read() in Child SIGUSR2");
        //    exit(-1);
        }
    }
}



void daemonize() {

    int fd0,fd1,fd2;
    struct sigaction sa;
    

    pid_t dpid;
    dpid = fork();
    if (dpid < 0) {
        exit(EXIT_FAILURE); 
    }
    
    //close parent process
    if (dpid > 0) {
        exit(EXIT_SUCCESS); 
    }

    //become session leader
    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }

    //disconnect with tty
    dpid = fork();
    if (dpid < 0) {
        exit(EXIT_FAILURE);
    }
    if (dpid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);
    if(chdir("/")<0){
	perror("cd()");
    }

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if(sigaction(SIGHUP,&sa,NULL)<0){
	perror("sigaction(): Can't ignore SIGHUP");
    }

    fd0 = open("/dev/null",O_RDWR);
    fd1 = dup(0);
    fd2 = dup(1);

    openlog("MiniProject2_Server", LOG_PID, LOG_DAEMON);
    //if(fd0 !=0||fd1!=1||fd2!=2){
//	syslog(LOG_ERR,"unexpected file descriptors %d %d %d", fd0, fd1, fd2);
//	exit(EXIT_FAILURE);
    //}
    syslog(LOG_INFO,"Daemon Process of Chat Server");

}



int main(int argc, char** argv) {
    int ssock;
    socklen_t clen;
    int n;
    struct sockaddr_in servaddr, cliaddr;
    char mesg[BUFSIZ];
    daemonize();
    if ((ssock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));

    if (bind(ssock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind()");
        close(ssock);
        return -1;
    }

    if (listen(ssock, 50) < 0) {
        perror("listen()");
        close(ssock);
        return -1;
    }

    int status;
    struct sigaction sa;

    signal(SIGUSR1, sigHandler1);
    signal(SIGUSR2, sigHandler2);

    do {
        // Parent Process
        conn[cnum].sfd = accept(ssock, (struct sockaddr*)&cliaddr, &clen);
        if (conn[cnum].sfd < 0) {
            perror("accept()");
            close(ssock);
            return -1;
        }


        // Create pipes
        if (pipe(conn[cnum].pfd_StoC) < 0 || pipe(conn[cnum].pfd_CtoS) < 0) {
            perror("pipe()");
            close(conn[cnum].sfd);
            close(ssock);
            return -1;
        }

        // Fork
        pid = fork();
        conn[cnum].pid = pid;

        if (pid > 0) {
            cnum++;
        }

        if (pid < 0) {
            perror("fork()");
            close(conn[cnum].sfd);
            close(conn[cnum].pfd_StoC[0]);
            close(conn[cnum].pfd_StoC[1]);
            close(conn[cnum].pfd_CtoS[0]);
            close(conn[cnum].pfd_CtoS[1]);
            close(ssock);
            return -1;
        } else if (pid == 0) {
            // Child Process
            conn[cnum].pid = getpid();

            do {
                P2P_MSG_t mesg_CtoP;
                if ((n = read(conn[cnum].sfd, &mesg_CtoP, sizeof(P2P_MSG_t))) < 0) {
                    perror("read()");
                    close(conn[cnum].sfd);
                    close(conn[cnum].pfd_StoC[0]);
                    close(conn[cnum].pfd_StoC[1]);
                    close(conn[cnum].pfd_CtoS[0]);
                    close(conn[cnum].pfd_CtoS[1]);
                    exit(-1);
                }

                mesg_CtoP.cnum = cnum;

		if (strcmp(mesg_CtoP.mesg,GOODBYE)==0){
		    write(conn[cnum].pfd_CtoS[1], &mesg_CtoP, sizeof(P2P_MSG_t));
		    kill(getppid(),SIGUSR1);
		    send(conn[cnum].sfd, &mesg_CtoP, sizeof(P2P_MSG_t), MSG_DONTWAIT);
		    break;
		}

                if (write(conn[cnum].pfd_CtoS[1], &mesg_CtoP, sizeof(P2P_MSG_t)) <= 0) {
                    perror("write()");
                    close(conn[cnum].sfd);
                    close(conn[cnum].pfd_StoC[0]);
                    close(conn[cnum].pfd_StoC[1]);
                    close(conn[cnum].pfd_CtoS[0]);
                    close(conn[cnum].pfd_CtoS[1]);
                    exit(-1);
                }

                kill(getppid(), SIGUSR1);
            } while(1);

            // Close file descriptors in child process
            close(conn[cnum].sfd);
            close(conn[cnum].pfd_StoC[0]);
            close(conn[cnum].pfd_StoC[1]);
            close(conn[cnum].pfd_CtoS[0]);
            close(conn[cnum].pfd_CtoS[1]);
            exit(0);
        }
	close(conn[cnum].sfd);
    } while(1);

    // Close file descriptors in parent process
    close(ssock);
    closelog();
    return 0;
}

