#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>
#include <ctype.h>
#define TCP_PORT 5100
#include "Server.h"

#define RESET   "\x1b[0m"
#define BLUE    "\x1b[34m"
#define GREEN   "\x1b[32m"

#define GOODBYE "Peace out"
int pfd_PtoC[2];
int pfd_CtoP[2];
int ssock;
char username[21];

int isValidUsername(const char *username) {
    int len = strlen(username);
    
    // Check name length
    if (len < 1 || len > 20) {
        return 0;
    }
    return 1;
}

void printMessage(const char *p_username, const char *message, const char* color) {
    // Get current time
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);

    // Time formatting
    char time_str[10];
    strftime(time_str, sizeof(time_str), "%I:%M %p", local);

    // Print Message
    printf("╔════════════════════════════╗\n");
    printf("║ %s[%s] %s(%s):\n",color, p_username,RESET,time_str);
    printf("║ %s\n", message);
    printf("╚════════════════════════════╝\n\n");
}

void sigHandler(int signo) {
    P2P_MSG_t MSG;
    if (signo == SIGUSR1) {
        ssize_t bytesRead = read(pfd_PtoC[0], &MSG,sizeof(P2P_MSG_t));
        if (bytesRead > 0) {

	    if(strcmp(MSG.username,username)==0)printMessage(MSG.username,MSG.mesg,GREEN);
	    else printMessage(MSG.username,MSG.mesg,RESET);
        }
    } else if (signo == SIGUSR2) {
        ssize_t bytesRead = read(pfd_CtoP[0], &MSG, sizeof(P2P_MSG_t));
        if (bytesRead > 0) {
            if(send(ssock, &MSG,sizeof(P2P_MSG_t) , MSG_DONTWAIT)<0){
		perror("send() in client sig2");
		exit(-1);
	    }
        }
    }
}
int main(int argc, char** argv) {
    int n;
    if (argc < 2) {
        printf("You need to input IP address as an argument\n");
        return -1;
    }
    struct sockaddr_in servaddr;
    if ((ssock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        return -1;
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &(servaddr.sin_addr.s_addr));
    servaddr.sin_port = htons(atoi(argv[2]));
    if (pipe(pfd_PtoC) < 0 || pipe(pfd_CtoP) < 0) {
        perror("pipe()");
        close(ssock);
        return -1;
    }
    if (connect(ssock, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect()");
        close(pfd_PtoC[0]);
        close(pfd_PtoC[1]);
        close(pfd_CtoP[0]);
        close(pfd_CtoP[1]);
        close(ssock);
        return -1;
    }

    //log-in
    int login = 1;
    while(login){
	printf("Please Enter username shorter than 20 Chararctors\n:");
	fgets(username, sizeof(username), stdin);

	username[strcspn(username, "\n")] = 0;

	if (isValidUsername(username)) {
            printf("Welcome, %s\n", username);
            break;
        } else {
            printf("Invalid username. Please try again.\n");
        }
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork()");
        close(pfd_PtoC[0]);
        close(pfd_PtoC[1]);
        close(pfd_CtoP[0]);
        close(pfd_CtoP[1]);
        close(ssock);
        return -1;
    }
    if (pid == 0) {
        signal(SIGUSR1, sigHandler);
        while (1) {
            char mesg[BUFSIZ];
            if (fgets(mesg, BUFSIZ, stdin) == NULL) {
                perror("fgets()");
                break;
            }

	    //clear input line
	    printf("\033[A\033[2K");

            if (strncmp(mesg, "quit", 4) == 0) {
                
		
		P2P_MSG_t mesg_CtoP;
		mesg_CtoP.pid = getpid();
		strcpy(mesg_CtoP.mesg,GOODBYE);

		//mesg_CtoP.mesg[n] = '\0';
		mesg_CtoP.cnum = 0;
		strcpy(mesg_CtoP.username,username);
		write(pfd_CtoP[1], &mesg_CtoP, sizeof(P2P_MSG_t));
		
		kill(getppid(), SIGUSR2);
		break;
            }
	    n = strlen(mesg);
            P2P_MSG_t mesg_CtoP;
            mesg_CtoP.pid = getpid();
            strncpy(mesg_CtoP.mesg, mesg, n);
            mesg_CtoP.mesg[n] = '\0';
            mesg_CtoP.cnum = 0;
	    strcpy(mesg_CtoP.username,username);
	   
            if(write(pfd_CtoP[1], &mesg_CtoP, sizeof(P2P_MSG_t))==-1){
		perror("write()");
		return -1;
	    }
	   
            kill(getppid(), SIGUSR2);
        }
        close(pfd_CtoP[1]);
	//printf("Child Process Dead\n");
        exit(0);
    } else {
        signal(SIGUSR2, sigHandler);
        while (1) {
            P2P_MSG_t MSG;
            ssize_t len = recv(ssock, &MSG, sizeof(P2P_MSG_t), 0);
            if (len <= 0) {
                perror("recv()");
                break;
            }

	    if(strcmp(MSG.mesg,GOODBYE)==0){
		//printf("I got a Peace out..let's see if it's mine.\n");
		if(strcmp(MSG.username,username)==0){
		   //printf("Shit it's mine. I'll die\n");
		    break;
		}
	    }

            write(pfd_PtoC[1], &MSG, sizeof(P2P_MSG_t));
            kill(pid, SIGUSR1);
            /* Check if the received message is "killall" to break the loop and logout
            if (strncmp(MSG.mesg, "killall", 7) == 0) {
		printf("kill all!\n");
                break;
            }*/
        }
        close(pfd_PtoC[1]);
    }
    close(pfd_PtoC[0]);
    close(pfd_CtoP[0]);
    close(ssock);
    return 0;
}
