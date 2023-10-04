#include "fifocommon.h"

/* server fifo name char array */
static char server_fifo[SERVER_FIFO_LEN];
/* terminate the program using this flag */
volatile sig_atomic_t terminate = 0;

/* semaphore declarations */
sem_t* sem_arr[50][2];
char file_names[50][50];
sem_t* client_sem;
sem_t* sem_sem;
sem_t* log_sem;

int sem_number = 0;

/* number of readers */
int reader = 0;

/* number of max clients */
int MAX_CLIENTS = 0;

/* server dir path */
char path[50];

/* client que */
pid_t clients[DEFAULT_SIZE];
int num_client = 0;

/* waiting que */
pid_t waiting[DEFAULT_SIZE];
int waiting_num = 0;

/* log filename */
char log_file[50];

/* file operations from fileop.c */
void list_op(char output[]);
void readF(char tokens[4][50], char resp[], int count);
void writeT(char tokens[4][50], char resp[], int count);
int upload_op(char tokens[4][50], char path[50]);
int download_op(char tokens[4][50], char path[50]);
int isValidNumber(char const str[]);
int tokenization(char arr[], char tokens[4][50]);
void help_op(char command[50], char resp[]);
void log_print(char filename[], pid_t pid, char command[]);

/* signal handler */
void handler(int signum)
{
    switch(signum)
    {
        case SIGINT:
            puts("\nSIGINT is received. Terminating...\n");
            terminate = 1;
            break;
        case SIGTERM:
            puts("\nSIGTERM is received. Terminating...\n");
            terminate = 1;
            break;
        case SIGUSR1:
            puts("Kill signal is received. Terminating...\n");
            terminate = 1;
            break;
    }
}

/* check if there is at least one client waiting */
/* if so, then add it to the client que */
void check_que()
{
    if(waiting_num != 0)
    {
        char temp[50];
        sprintf(temp, "/%ld", (long) waiting[0]);
        sem_t* sem = sem_open(temp, 0);
        if(sem == SEM_FAILED) 
        {
            perror("sem_open failed");
            exit(1);
        }

        /* wake up the client */
        if(sem_post(sem) == -1) 
        {
            perror("sem_post failed");
            exit(1);
        }

        clients[num_client++] = waiting[0];

        printf("Client PID %ld connected\n", (long)waiting[0]);
        fflush(stdout);

        for(int i = 0; i < waiting_num; i++)
        {
            if(i != waiting_num-1)
                waiting[i] = waiting[i+1];
        }

        waiting_num--;

    }
}

/* check if the given file has a semaphore */
int is_sem_available(char name[])
{
    for(int i = 0; i < sem_number; i++)
    {
        if(strcmp(name, file_names[i]) == 0)
            return i;
    }

    return -1;
}

/* check if the given pid is in the array */
int is_here(pid_t pid, pid_t arr[], int num)
{
    for(int i = 0; i < num; i++)
    {
        if(arr[i] == pid)
            return 1;
    }
    return 0;
}

/* remove the given pid from the array */
void remove_client(pid_t pid, pid_t arr[], int num)
{
    int flag = 0;
    for(int i = 0; i < num; i++)
    {
        if(arr[i] == pid)
        {
            flag = 1;
            if(i != num-1)
                arr[i] = arr[i+1];
        }
        else if(flag == 1 && i != num-1)
            arr[i] = arr[i+1];
    }
}

void operations(char input[], struct response *resp)
{
    char temp[100];
    char tokens[4][50];
    strcpy(temp, input);
    int count = tokenization(temp, tokens);


    if(strcmp("help", tokens[0]) == 0 && (count == 1 || count == 2))
    {
        if(count == 1)
            strcpy(resp->message, "\nAvailable comments are : help, list, readF, writeT, upload, download, quit, killServer"); 
        else 
        {
            char output[LEN_MES] = "";
            help_op(tokens[1], output);
            strcpy(resp->message, output);
        }
    }
    else if(strcmp("list", input) == 0)
    {
        char output[LEN_MES] = "";
        list_op(output);
        strcpy(resp->message, output); 
    }
    else if(strcmp("readF", tokens[0]) == 0 && (count == 3 || count == 2))
    {
        char output[LEN_MES] = "";

        int ind = is_sem_available(tokens[1]);
        if(ind == -1)
        {
            sem_wait(sem_sem);
            strcpy(file_names[sem_number], tokens[1]);

            ind = sem_number;
            sem_number++;

            sem_post(sem_sem);
        }

        /* reader part */

        if(sem_wait(sem_arr[ind][0]) == -1)
           perror("sem_wait");

        reader++;

        if(reader == 1)
        {
            if(sem_wait(sem_arr[ind][1]) == -1)
                perror("sem_wait");
        }

        if(sem_post(sem_arr[ind][0]) == -1)
           perror("sem_post");

        readF(tokens, output, count);

        if(sem_wait(sem_arr[ind][0]) == -1)
           perror("sem_wait");

        reader--;

        if(reader == 0)
        {
            if(sem_post(sem_arr[ind][1]) == -1)
                perror("sem_post");
        }

        if(sem_post(sem_arr[ind][0]) == -1)
           perror("sem_post");

        strcpy(resp->message, output); 
    }
    else if(strcmp("writeT", tokens[0]) == 0 && (count == 3 || count == 4))
    {
        char output[LEN_MES] = "";

        int ind = is_sem_available(tokens[1]);
        if(ind == -1)
        {
            sem_wait(sem_sem);
            strcpy(file_names[sem_number], tokens[1]);

            ind = sem_number;
            sem_number++;

            sem_post(sem_sem);
        }

        /* writer part */

        if(sem_wait(sem_arr[ind][1]) == -1)
           perror("sem_wait");

        writeT(tokens, output, count);

        if(sem_post(sem_arr[ind][1]) == -1)
           perror("sem_post");
 
        strcpy(resp->message, output); 
    }
    else if(strcmp("upload", tokens[0]) == 0 && count == 2)
    {
        int ind = is_sem_available(tokens[1]);
        if(ind == -1)
        {
            sem_wait(sem_sem);
            strcpy(file_names[sem_number], tokens[1]);

            ind = sem_number;
            sem_number++;

            sem_post(sem_sem);
        }

        /* reader part */

        if(sem_wait(sem_arr[ind][0]) == -1)
           perror("sem_wait");

        reader++;

        if(reader == 1)
        {
            if(sem_wait(sem_arr[ind][1]) == -1)
                perror("sem_wait");
        }

        if(sem_post(sem_arr[ind][0]) == -1)
           perror("sem_post");

        int res_op = upload_op(tokens, path);

        if(sem_wait(sem_arr[ind][0]) == -1)
           perror("sem_wait");

        reader--;

        if(reader == 0)
        {
            if(sem_post(sem_arr[ind][1]) == -1)
                perror("sem_post");
        }

        if(sem_post(sem_arr[ind][0]) == -1)
           perror("sem_post");

        if(res_op == 0)
            strcpy(resp->message, "\nFile has been uploaded."); 
        else
            strcpy(resp->message, "\nFile is not found."); 
    }
    else if(strcmp("download", tokens[0]) == 0 && count == 2)
    {
        int ind = is_sem_available(tokens[1]);
        if(ind == -1)
        {
            sem_wait(sem_sem);
            strcpy(file_names[sem_number], tokens[1]);

            ind = sem_number;
            sem_number++;

            sem_post(sem_sem);
        }

        /* reader part */

        if(sem_wait(sem_arr[ind][0]) == -1)
           perror("sem_wait");

        reader++;

        if(reader == 1)
        {
            if(sem_wait(sem_arr[ind][1]) == -1)
                perror("sem_wait");
        }

        if(sem_post(sem_arr[ind][0]) == -1)
           perror("sem_post");

        int res_op = download_op(tokens, path);

        if(sem_wait(sem_arr[ind][0]) == -1)
           perror("sem_wait");

        reader--;

        if(reader == 0)
        {
            if(sem_post(sem_arr[ind][1]) == -1)
                perror("sem_post");
        }

        if(sem_post(sem_arr[ind][0]) == -1)
           perror("sem_post");

        if(res_op == 0)
            strcpy(resp->message, "\nFile has been downloaded."); 
        else
            strcpy(resp->message, "\nFile is not found."); 
    }
    else if(strcmp("quit", input) == 0)
    {
        strcpy(resp->message, "\nDisconnected, bye"); 
    }
    else if(strcmp("killServer", input) == 0)
    {
        strcpy(resp->message, "\nServer is terminated, bye"); 
    }
    else
    {
        strcpy(resp->message, "\nThis command is not available, type help to see available ones."); 
    }
}

void execute_server()
{
    int server_fd, dummy_fd, client_fd;

    /* create all semaphores beforehand */
    client_sem = sem_open("/client", O_CREAT, 0644, 1);
    sem_sem = sem_open("/sem", O_CREAT, 0644, 1);
    log_sem = sem_open("/log", O_CREAT, 0644, 1);

    if(client_sem == SEM_FAILED || sem_sem == SEM_FAILED || log_sem == SEM_FAILED) 
    {
        perror("Failed to create semaphores");
        exit(1);
    }

    char temp1[8], temp2[8];
    for(int i = 0; i < 50; i++)
    {
        sprintf(temp1, "/m%d", i);
        sprintf(temp2, "/w%d", i);
        sem_arr[i][0] = sem_open(temp1, O_CREAT, 0644, 1);
        sem_arr[i][1] = sem_open(temp2, O_CREAT, 0644, 1);
    }

    /* create server fifo */
    umask(0);
    snprintf(server_fifo, SERVER_FIFO_LEN, SERVER_FIFO, (long) getpid());
    if(mkfifo(server_fifo, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST)
    {
        perror("mkfifo error");
        exit(1);
    }
    
    server_fd = open(server_fifo, O_RDONLY);
    if(server_fd == -1)
    {
        perror("server_fd open error");
        exit(1);
    }

    dummy_fd = open(server_fifo, O_WRONLY);
    if(dummy_fd == -1)
    {
        perror("dummy_fd open error");
        exit(1);
    }
    
    if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        perror("signal error");
        exit(1);
    }
    
    while(terminate == 0)
    {
        char client_fifo[CLIENT_FIFO_LEN];
        struct request req;
        struct response resp;
        pid_t f;

        if(read(server_fd, &req, sizeof(struct request)) != sizeof(struct request))
        {
            if(terminate == 0)
            {
                perror("read request error");
                exit(1);
            }
            else
                break;
        }

        /* check if the client is already in que */
        /* if not, then check the number of clients */
        /* if there is a slot, then add the client, otherwise add it to waiting que */
        if(is_here(req.pid, clients, num_client) == 0)
        {
            sem_wait(client_sem);
            if(num_client < MAX_CLIENTS)
            {
                clients[num_client++] = req.pid;

                printf("Client PID %ld connected\n", (long)req.pid);
                fflush(stdout);

                snprintf(client_fifo, CLIENT_FIFO_LEN, CLIENT_FIFO, (long)req.pid);
                client_fd = open(client_fifo, O_WRONLY);
                if(client_fd == -1)
                {
                    perror("client_fd open error");
                    exit(1);
                }
                strcpy(resp.message, "added");
                if(write(client_fd, &resp, sizeof(struct response)) != sizeof(struct response))
                    fprintf(stderr, "error writing to fifo %s\n", client_fifo);
                if(close(client_fd) == -1)
                {
                    perror("client_fd close error");
                    exit(1);
                }
            }
            else
            {
                printf("Connection request PID %ld . Que is full!\n", (long)req.pid);
                fflush(stdout);

                snprintf(client_fifo, CLIENT_FIFO_LEN, CLIENT_FIFO, (long) req.pid);
                client_fd = open(client_fifo, O_WRONLY);
                if(client_fd == -1)
                {
                    perror("client_fd open error");
                    exit(1);
                }
                strcpy(resp.message, "not added");
                if(write(client_fd, &resp, sizeof(struct response)) != sizeof(struct response))
                    fprintf(stderr, "error writing to fifo %s\n", client_fifo);
                if(close(client_fd) == -1)
                {
                    perror("client_fd close error");
                    exit(1);
                }

                if(strcmp(req.command, "1") == 0)
                    waiting[waiting_num++] = req.pid;
            }
            
            sem_post(client_sem);
            continue;
        }

        /* create a child process to execute the command */
        f = fork();

        if(f > 0) 
        {
            int status;
            pid_t child_pid;

            do 
            {
                child_pid = wait(&status);
            } 
            while (child_pid == -1 && errno == EINTR);
        }
        else if(f == 0) 
        {

            operations(req.command, &resp);

            snprintf(client_fifo, CLIENT_FIFO_LEN, CLIENT_FIFO, (long) req.pid);
            client_fd = open(client_fifo, O_WRONLY);
            if(client_fd == -1)
            {
                perror("client_fd 1open error");
                exit(1);
            }

            if(write(client_fd, &resp, sizeof(struct response)) != sizeof(struct response))
                fprintf(stderr, "error writing to fifo %s\n", client_fifo);
            if(close(client_fd) == -1)
            {
                perror("client_fd close error");
                exit(1);
            }

            if(strcmp(req.command, "killServer") == 0)
            {
                printf("From Client PID %ld, ", (long)req.pid);
                fflush(stdout);
                kill(getppid(), SIGUSR1);
            }
        
            exit(0);
        }
        else
        {
            perror("fork failed");
            exit(1);
        }

        /* add the command to the log file */
        sem_wait(log_sem);
        log_print(log_file, req.pid, req.command);
        sem_post(log_sem);
        
        /* if the client exits, then remove it and add the next client in the waiting que */
        if(strcmp(req.command, "quit") == 0)
        {
            sem_wait(client_sem);
            remove_client(req.pid, clients, num_client);
            num_client--;
            printf("Client PID %d disconnected..\n", req.pid);
            fflush(stdout);

            check_que();
            sem_post(client_sem);
        }
    }

    /* terminate all clients when server is terminated */
    for(int i = 0; i < num_client; i++)
        kill(clients[i], SIGTERM);
    for(int i = 0; i < waiting_num; i++)
    {
        kill(waiting[i], SIGTERM);
        char temp[50];
        sprintf(temp, "/%ld", (long) waiting[0]);
        sem_t* sem = sem_open(temp, 0);
        if (sem == SEM_FAILED) {
            perror("sem_open failed");
            exit(1);
        }
        if (sem_post(sem) == -1) {
            perror("sem_post failed");
            exit(1);
        }
    }

    
    /* close and unlink part */
    if(close(server_fd) == -1)
    {
        perror("server_fd close error");
        exit(1);
    }
    if(close(dummy_fd) == -1)
    {
        perror("dummy_fd close error");
        exit(1);
    }
    if(unlink(server_fifo) == -1)
    {
        perror("unlink server error");
        exit(1);
    }

    if(sem_close(client_sem) == -1)
    {
        perror("close writer error");
        exit(1);
    }
    if(sem_close(sem_sem) == -1)
    {
        perror("close writer error");
        exit(1);
    }
    if(sem_close(log_sem) == -1)
    {
        perror("close writer error");
        exit(1);
    }

    if(sem_unlink("/client") == -1)
    {
        perror("unlink writer error");
        exit(1);
    }
    if(sem_unlink("/sem") == -1)
    {
        perror("unlink writer error");
        exit(1);
    }
    if(sem_unlink("/log") == -1)
    {
        perror("unlink writer error");
        exit(1);
    }


    for(int i = 0; i < sem_number; i++)
    {
        if(sem_close(sem_arr[i][0]) == -1)
        {
            perror("close writer error");
            exit(1);
        }
        if(sem_close(sem_arr[i][1]) == -1)
        {
            perror("close writer error");
            exit(1);
        }

        sprintf(temp1, "/m%d", i);
        sprintf(temp2, "/w%d", i);

        if(sem_unlink(temp1) == -1)
        {
            perror("unlink writer error");
            exit(1);
        }
        if(sem_unlink(temp2) == -1)
        {
            perror("close writer error");
            exit(1);
        }
    }

    exit(0);
    
}

/* change the working directory */
void change_dir(char const *name)
{
    if(access(name, F_OK) != 0) 
    {
        if(mkdir(name, 0777) == -1) //read, write, execute allowed
        {
            perror("creating directory error");
            exit(1);
        }
    } 

    if(chdir(name) == -1) 
    {
        perror("changing directory error");
        exit(1);
    }
}

int main(int argc, char const *argv[])
{
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
    if(sigaction(SIGUSR1, &sa, NULL) == -1) 
    {
        perror("sigaction error");
        exit(1);
    }


    if(argc == 3)
    {
        if(isValidNumber(argv[2]) == 1)
        {
            change_dir(argv[1]); /* change the working directory */
            MAX_CLIENTS = atoi(argv[2]); /* set the max client number */
            printf("\nServer with pid %ld has started to run, waiting for clients\n", (long) getpid());
            
            /* store the working directory of server */
            strcpy(path ,argv[1]);

            /* create the log file */
            sprintf(log_file, "log_%ld.txt", (long) getpid());
            int fd = open(log_file, O_WRONLY | O_CREAT, 0666);
            if (fd == -1) 
            {
                perror("file open error (log).");
                exit(1);
            }
            close(fd);

            /* start the server */
            execute_server();
        }
        else
        {
            perror("invalid argument for #clients");
            return 0;
        }
    }
    else
    {
        perror("invalid argument number");
        return 0;
    }

    return 0;
}

