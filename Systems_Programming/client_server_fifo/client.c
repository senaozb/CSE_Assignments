#include "fifocommon.h"

/* client fifo name char array */
static char client_fifo[CLIENT_FIFO_LEN];
/* terminate the program using this flag */
volatile sig_atomic_t terminate = 0;
/* SIGINT flag */
int sigint_flag = 0;

/* signal handler */
void handler(int signum)
{
    switch(signum)
    {
        case SIGINT:
            terminate = 1;
            sigint_flag = 1;
            break;
        case SIGTERM:
            terminate = 1;
            break;
    }
}

/* execute the client side */
void execute_client(int flag, const char serverpid[])
{
    int server_fd, client_fd;
    struct request req;
    struct response resp; 
    char input[100];
    char server_fifo[SERVER_FIFO_LEN];
    int wait_flag;

    /* create the client fifo according to its pid */
    umask(0);
    snprintf(client_fifo, CLIENT_FIFO_LEN, CLIENT_FIFO, (long) getpid());
    if(mkfifo(client_fifo, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST)
    {
        perror("mkfifo error");
        exit(1);
    }

    /* assign its pid to pid in request */
    req.pid = getpid();

    snprintf(server_fifo, SERVER_FIFO_LEN, SERVER_FIFO, (long) atoi(serverpid));
    server_fd = open(server_fifo, O_WRONLY);
    if(server_fd == -1)
    {
        perror("server_fd open error");
        exit(1);
    }

    printf("\nWaiting for Que..\n");
    fflush(stdout);

    /* send the flag for preference to server */
    char str[2];
    sprintf(str,"%d",flag);
    strcpy(req.command, str);

    /* send a message to send the request at first */
    if(write(server_fd, &req, sizeof(struct request)) != sizeof(struct request))
    {
        perror("write to server error");
        exit(1);
    }

    client_fd = open(client_fifo, O_RDONLY);
    if(client_fd == -1)
    {
        perror("client_fd open error");
        exit(1);
    }

    /* receive the message */
    if(read(client_fd, &resp, sizeof(struct response)) != sizeof(struct response))
    {
        if(errno != EAGAIN) 
        {
            perror("error reading from client");
            exit(1);
        }
    }

    if(close(client_fd) == -1)
    {
        perror("client_fd close error");
        exit(1);
    }

    /* if the que is full, then take action regarding preference */
    if(strcmp(resp.message, "not added") == 0)
    {
        /* if it is tryConnect, then terminate it */
        if(flag == 2)
        {
            printf("Que is full. Terminating...\n");
            fflush(stdout);
        }
        else /* if it is Connect, then wait for the slots */
        {
            /* semaphore is also controlled by the server */
            char temp[50];
            sprintf(temp, "/%ld", (long) getpid());
            sem_t* sem = sem_open(temp, O_CREAT, 0644, 0);
            if (sem == SEM_FAILED) {
                perror("sem_open failed");
                exit(1);
            }

            if (sem_wait(sem) == -1) {
                if(terminate == 1)
                {
                    printf("Server is terminated, bye\n");
                    fflush(stdout);
                }
                else
                {
                    perror("sem_wait failed");
                    exit(1);
                }
            }

            if(terminate == 0)
                wait_flag = 1; /* when server calls sem_post, stop waiting and go to the command loop */
        
            if(sem_close(sem) == -1)
            {
                perror("close writer error");
                exit(1);
            }
            if(sem_unlink(temp) == -1)
            {
                perror("unlink writer error");
                exit(1);
            }
        }
        
    }

    /* if there is a slot available, then server sends "added" and clients is connected to server */
    /* or after waiting for the empty slot, client side continues to execute */
    if(strcmp(resp.message, "added") == 0 || wait_flag == 1)
    {
        printf("Connection established.\n");
        fflush(stdout);
        
        /*infinite loop to get requests from the client*/
        while(terminate == 0)
        {
            /* get the command */
            printf(">>Enter the command: ");
            fflush(stdout);

            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0';
            strcpy(req.command, input);

            /* if SIGTERM is received, then break the loop */
            if(terminate == 1)
            {
                if(sigint_flag == 0)
                {
                    printf("\nServer is terminated, bye \n");
                    fflush(stdout);
                }
                break;
            }
            
            /* write the request to server fifo */
            if(write(server_fd, &req, sizeof(struct request)) != sizeof(struct request))
            {
                perror("write to server error");
                exit(1);
            }

            client_fd = open(client_fifo, O_RDONLY);
            if(client_fd == -1)
            {
                perror("client_fd open error");
                exit(1);
            }

            /* read the response from client fifo */
            if(read(client_fd, &resp, sizeof(struct response)) != sizeof(struct response))
            {
                if(errno != EAGAIN) 
                {
                    perror("error reading from client");
                    exit(1);
                }
            }
            
            if(close(client_fd) == -1)
            {
                perror("client_fd close error");
                exit(1);
            }

            printf("%s\n", resp.message);
            fflush(stdout);

            /* break the loop to terminate the program */
            if(strcmp(input, "quit") == 0 || strcmp(input, "killServer") == 0)
                break;
        }
    }
    
    /* if SIGINT is received, then send a quit command to server so that it removes the client */
    if(sigint_flag == 1)
    {
        strcpy(req.command, "quit");
        if(write(server_fd, &req, sizeof(struct request)) != sizeof(struct request))
        {
            perror("write to server error");
            exit(1);
        }

        client_fd = open(client_fifo, O_RDONLY);
        if(client_fd == -1)
        {
            perror("client_fd open error");
            exit(1);
        }

        /* read the response from client fifo */
        if(read(client_fd, &resp, sizeof(struct response)) != sizeof(struct response))
        {
            if(errno != EAGAIN) 
            {
                perror("error reading from client");
                exit(1);
            }
        }
        
        if(close(client_fd) == -1)
        {
            perror("client_fd close error");
            exit(1);
        }

        printf("%s\n", resp.message);
        fflush(stdout);
    }

    if(close(server_fd) == -1) 
    {
        perror("Error closing server FIFO");
        exit(1);
    }
    
    unlink(client_fifo);

    exit(0);
}

/* check if the given string is a valid number */
int isValidNumber(char const str[])
{
    int i = 0;
    while(str[i] != '\0')
    {
        if(isdigit(str[i]) == 0)
            return 0;
        i++;
    }
    return 1;
}

int main(int argc, char const *argv[])
{
    int flag = 0;
    
    /* Signals */
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    /* set the signals */
    if(sigaction(SIGINT, &sa, NULL) == -1) 
    {
        perror("sigaction error");
        exit(1);
    }
    if(sigaction(SIGTERM, &sa, NULL) == -1) 
    {
        perror("sigaction error");
        exit(1);
    }

    if(argc == 3)
    {
        if(isValidNumber(argv[2]) != 1)
        {
            perror("invalid argument for server pid");
            return 0;
        }
       
        /*set the flag */
        if(strcmp("Connect", argv[1]) == 0)
            flag = 1;
        else if(strcmp("tryConnect", argv[1]) == 0)
            flag = 2;
        else
        {
            perror("invalid argument type");
            return 0;
        }
    
        execute_client(flag, argv[2]);

    }
    else
    {
        perror("invalid argument number");
        return 0;
    }

    return 0;
}
