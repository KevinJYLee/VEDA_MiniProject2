#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <wait.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

typedef struct connection {
    int sfd;
    int pfd_StoC[2];
    int pfd_CtoS[2];
    pid_t pid;
} connection_t;

typedef struct P2P_MSG{
    char mesg[BUFSIZ];
    pid_t pid;
    int cnum;
    char username[BUFSIZ];
} P2P_MSG_t;


