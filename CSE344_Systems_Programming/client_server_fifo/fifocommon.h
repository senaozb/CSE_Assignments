#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SERVER_FIFO "/tmp/system_sv.%ld"
#define CLIENT_FIFO "/tmp/system_cl.%ld"
#define SERVER_FIFO_LEN (sizeof(SERVER_FIFO) + 20)
#define CLIENT_FIFO_LEN (sizeof(CLIENT_FIFO) + 20)

#define LEN_MES 4000
#define DEFAULT_SIZE 1000

struct request 
{
    pid_t pid;
    char command[100];
};

struct response 
{
    char message[LEN_MES];
};
