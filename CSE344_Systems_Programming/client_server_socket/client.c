#include "common.h"

char dirname[DEFAULT_SIZE]; /* directory path of client */
char ip_add[16]; /* ip address as an argument */
int port_number; /* port number of server */
int client_fd;
struct sockaddr_in server_address;

int terminate = 0; /* termiante flag */
int exit_f = 0; /* exit flag for incoming messages */

/* log filename */
char log_file[400];

void log_print(char filename[], char side[], char command[])
{
    /* open the log file */
    int fd = open(filename, O_WRONLY | O_APPEND);
    if (fd == -1) {
        perror("Failed to open log file.");
        exit(1);
    }

    /* print the command */
    write(fd, command, strlen(command));
    write(fd, " from ", strlen(" from "));
    write(fd, side, strlen(side));
    write(fd, "\n", 1);

    /* Close the log file */    
    if(close(fd) == -1) 
    {
        perror("Error close");
        exit(1);
    } 
}

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
    }
}

/* create directory of the client */
void create_dir(char* path) 
{
    /* Check if the directory exists*/
    if(access(path, F_OK) == 0) 
    {
        pid_t pid = fork();

        if(pid == -1) 
        {
            perror("Error forking process");
            exit(1);
        } 
        else if(pid == 0) 
        {
            /* if it exists, then remove it to recreate it */
            char* argv[] = { "rm", "-rf", path, NULL };
            char* envp[] = { NULL };

            if(execve("/bin/rm", argv, envp) == -1) 
            {
                perror("Error executing command");
                exit(1);
            }
        } 
        else 
        {
            int status;
            pid_t result = waitpid(pid, &status, 0);

            if(result == -1) 
            {
                perror("Error waiting for child process");
                exit(1);
            }
        }
    } 

    /* Recreate the directory */
    if (mkdir(path, 0777) == -1) 
    {
        perror("Error creating directory");
        exit(1);
    }
}


/* read messages from the server */
int read_from_server(int fd, char messages[][1024], int *count)
{
    int flag = 0;
    char temp_file[DEFAULT_SIZE];
    FILE* file;

    while(terminate == 0)  /* check if a signal received */
    {
        char arr[BUFFER_SIZE];
        int bytes_received = recv(fd, arr, BUFFER_SIZE, 0);
        if (bytes_received == -1) 
        {
            if(terminate == 1)
                break;

            perror("receive failed");
            exit(1);
        }

        if(strcmp("END", arr) == 0) /* end is received, return */
        {
            if(flag == 1)
                fclose(file);
            return 0;
        }
        if(strcmp("EXIT", arr) == 0) /* server is terminated, terminate the program */
        {
            if(flag == 1)
                fclose(file);
            return 1;
        }
        if(strncmp(arr, "A1|", 3) == 0 || strncmp(arr, "M3|", 3) == 0) /* add or modify a file */
        {
            log_print(log_file, "server", arr);

            strcpy(temp_file, dirname);
            const char* delimiter = "|";

            /* get the file path */
            char* sub = strtok(arr, delimiter);
            sub = strtok(NULL, delimiter);
            
            char* token = NULL;
            char* result = NULL;

            /* change the beginning of the path with client's directory path */
            token = strchr(sub, '/');
            if(token != NULL) 
            {
                token++;
                result = strstr(sub, token);
                if(result == NULL) 
                {
                    printf("Invalid string format.\n");
                    fflush(stdout);
                    continue;
                }
            }
            else
            {
                printf("Invalid string format.\n");
                fflush(stdout);
                continue;
            }

            strcpy(temp_file, dirname);
            strcat(temp_file, "/");
            strcat(temp_file, result);

            /* add the message to this temp messages array */
            strcpy(messages[(*count)++], temp_file);

            if(flag == 1) /* if another file is open, then close it first */
            {
                int result = fclose(file);
                if(result != 0) 
                {
                    perror("Failed to close file");
                    exit(1);
                }
            }

            flag = 1;
            /* open the file with write mode */
            file = fopen(temp_file, "w");
            if (file == NULL) 
            {
                perror("Failed to open the file for writing.\n");
                exit(1);
            }

            int r = fclose(file);
            if(r != 0) 
            {
                perror("Failed to close file");
                exit(1);
            }

            /* re-open it with append mode to write the content */
            file = fopen(temp_file, "a");
            if (file == NULL) {
                perror("Failed to open the file for writing.\n");
                exit(1);
            }

        }
        else if(strncmp(arr, "D2|", 3) == 0) /* delete the file */
        {
            log_print(log_file, "server", arr);

            strcpy(temp_file, dirname);
            const char* delimiter = "|";

            char* sub = strtok(arr, delimiter);
            sub = strtok(NULL, delimiter);
            
            char* token = NULL;
            char* result = NULL;

            /* change the beginning of the path with client's directory path */
            token = strchr(sub, '/');
            if(token != NULL) 
            {
                token++;
                result = strstr(sub, token);
                if(result == NULL) 
                {
                    printf("Invalid string format.\n");
                    fflush(stdout);
                    continue;
                }
            }
            else
            {
                printf("Invalid string format.\n");
                fflush(stdout);
                continue;
            }


            strcpy(temp_file, dirname);
            strcat(temp_file, "/");
            strcat(temp_file, result);

            /* add the message to this temp messages array */
            strcpy(messages[(*count)++], temp_file);

            int res = remove(temp_file); /* remove the file */
            if (res != 0) 
            {
                perror("File removal failed");
                exit(1);
            }
        }
        else if(flag == 1) /* write the content of the file */
            fprintf(file, "%s", arr);
    }

    if(flag == 1) /* if there is a file opened, then close it */
    {
        int result = fclose(file);
        if (result != 0) 
        {
            perror("Failed to close file");
            return 1;
        }
    }


    return 0;
}

/* send messages from client to server */
void send_from_client(char arr[], int fd, char messages[][1024], int count, char filename_sent[DEFAULT_SIZE])
{
    /* check if the change is from client or server */
    for(int i = 0; i < count; i++)
    {
        if(strcmp(filename_sent, messages[i]) == 0)
            return;   
    }

    int bytes_sent = send(fd, arr, BUFFER_SIZE, 0);
    if (bytes_sent == -1) 
    {
        perror("send failed");
        exit(1);
    }

    if(strncmp(arr, "D2|", 3) == 0)
        log_print(log_file, "client", arr);

    if(strncmp(arr, "A1|", 3) == 0 || strncmp(arr, "M3|", 3) == 0) /* if a file is added or modified */
    {
        log_print(log_file, "client", arr);

        const char* delimiter = "|";

        /* get the path */
        char* sub = strtok(arr, delimiter);
        sub = strtok(NULL, delimiter);

        /* open it for reading */
        FILE* file = fopen(sub, "r");  
        if (file == NULL) 
        {
            perror("Failed to open the file");
            exit(1);
        }

        char line[BUFFER_SIZE];
        /* send the content line by line */
        while (fgets(line, BUFFER_SIZE, file) != NULL) 
        {
            int bytes_sent = send(fd, line, BUFFER_SIZE, 0);
            if (bytes_sent == -1) 
            {
                perror("send failed");
                exit(1);
            }
        }

        int result = fclose(file);
        if (result != 0) 
        {
            perror("Failed to close file");
            exit(1);
        }
    }
}

/* get the current directory state */
struct dir_state get_dir_state(const char* dir_path) 
{
    struct dir_state state;
    state.num_files = 0;
    state.files = NULL;

    DIR* dir = opendir(dir_path);
    if (dir == NULL) 
    {
        perror("Error opening directory");
        exit(1);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) 
    {
        if (entry->d_name[0] == '.' || (strncmp(entry->d_name, "log", 3) == 0)) /* skip the files starting with . */
            continue; 
        
        struct stat file_stat;
        char path[DEFAULT_SIZE];
        snprintf(path, DEFAULT_SIZE, "%s/%s", dir_path, entry->d_name);
        if (stat(path, &file_stat) == -1) 
        {
            perror("Error getting file information");
            exit(1);
        }

        if (S_ISREG(file_stat.st_mode) == 1) /* if it is a file*/
        {
            state.files = realloc(state.files, (state.num_files + 1) * sizeof(struct file_info));
            state.files[state.num_files].modified_time = file_stat.st_mtime;
            strcpy(state.files[state.num_files].path, path);
            state.num_files++;
        } 
        else if (S_ISDIR(file_stat.st_mode) == 1) /* if it is a directory */
            continue;
    }

    int result = closedir(dir);
    if (result == -1) 
    {
        perror("Failed to close directory");
        exit(1);
    }


    return state;
}

/* compare previous directory state to current one */
void check_dir(struct dir_state previous, struct dir_state current, int fd, char messages[][1024], int count) 
{
    char arr[BUFFER_SIZE] = "";

    /* compare previous to current states to find modified and removed files */
    for (int i = 0; i < previous.num_files; i++) 
    {
        int found = 0;
        for (int j = 0; j < current.num_files; j++) 
        {
            if (strcmp(previous.files[i].path, current.files[j].path) == 0) 
            {
                found = 1;
                if (previous.files[i].modified_time != current.files[j].modified_time) 
                {
                    strcpy(arr, "M3|"); /* file is modified */
                    strcat(arr, previous.files[i].path);
                    send_from_client(arr, fd, messages, count, previous.files[i].path);
                }
                break;
            }
        }
        if (found == 0) /* file is deleted */
        {
            strcpy(arr, "D2|");
            strcat(arr, previous.files[i].path);
            send_from_client(arr, fd, messages, count, previous.files[i].path);
        }
    }

    /* compare current to previous states to find added files*/
    for (int i = 0; i < current.num_files; i++) 
    {
        int found = 0;
        for (int j = 0; j < previous.num_files; j++) 
        {
            if (strcmp(current.files[i].path, previous.files[j].path) == 0) 
            {
                found = 1;
                break;
            }
        }
        if (found == 0) /* file is added */
        {
            strcpy(arr, "A1|");
            strcat(arr, current.files[i].path);
            send_from_client(arr, fd, messages, count, current.files[i].path);
        }
    }

    /* the message is ended with END */
    char a[BUFFER_SIZE] = "END";
    int bytes_sent = send(fd, a, BUFFER_SIZE, 0); 
    if (bytes_sent == -1) 
    {
        perror("send failed");
        exit(1);
    }

    free(previous.files);
}


void client_loop(int fd)
{
    struct dir_state previous, current;
    previous = get_dir_state(dirname);
    char messages[MAX_MESSAGE][1024];
    int count = 0;

    while(terminate == 0)
    {
        memset(messages, 0, sizeof(messages));
        count = 0;

        int exit_f = read_from_server(fd, messages, &count); /* get messages from server */
        if(exit_f == 1) /* if server is terminated, then terminate the program */
        {
            printf("Server is terminated. Bye!\n");
            fflush(stdout);
            return;
        }
    
        sleep(5);

        /* send a message to quit if a signal is caught */
        if(terminate == 1)
        {
            char arr[BUFFER_SIZE] = "EXIT";
            if (send(client_fd, arr, BUFFER_SIZE, 0) == -1) 
            {
                perror("Send failed");
                exit(1);
            }
            printf("Program is terminated. Bye \n");
            fflush(stdout);
        }

        char arr[BUFFER_SIZE];
        current = get_dir_state(dirname); /* get the current directory state */
        check_dir(previous, current, client_fd, messages, count); /* compare them */
        previous = current; /* update the previous one */

        /* send a message to quit if a signal is caught */
        if(terminate == 1)
        {
            strcpy(arr, "EXIT");
            int bytes_sent = send(client_fd, arr, BUFFER_SIZE, 0);
            if (bytes_sent == -1) 
            {
                perror("send failed");
                exit(1);
            }
        }
    }
}

int sync_dir(int fd)
{
    /* get the current directory from server */
    int count = 0;
    char messages[MAX_MESSAGE][1024];
    int exit_f = read_from_server(fd, messages, &count);
    if(exit_f == 1)
    {
        printf("Server is terminated. Bye!\n");
        fflush(stdout);
        return 1;
    }

    return 0;
}

void client() 
{
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons((u_short)port_number);
    server_address.sin_addr.s_addr = inet_addr(ip_add);

    /* create the socket */
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) 
    {
        perror("Socket creation failed");
        exit(1);
    }

    /* send request to server to connect */
    if (connect(client_fd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) 
    {
        perror("Connection failed");
        exit(1);
    }

    printf("Waiting for the connection\n");
    fflush(stdout);

    /* Check if server is full */
    char ans[BUFFER_SIZE];
    int bytes_received = recv(client_fd, ans, BUFFER_SIZE, 0);
    if (bytes_received == -1) 
    {
        perror("receive failed");
        exit(1);
    }

    if(strcmp(ans, "FULL") == 0)
    {
        printf("Server is full. Terminating...\n");
        fflush(stdout);
        /* Close socket */
        int result = close(client_fd);
        if (result == -1) 
        {
            perror("Failed to close client file descriptor");
            exit(1);
        }
        return;
    }
    
    printf("Connected to server\n");
    fflush(stdout);

    if(sync_dir(client_fd) != 1)
        client_loop(client_fd);
   

    /* Close socket */
    int result = close(client_fd);
    if (result == -1) 
    {
        perror("Failed to close client file descriptor");
        exit(1);
    }
}

/* check if it is a valid number */
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

    if(argc != 4)
    {
        perror("invalid argument number");
        exit(1);
    }

    if(isValidNumber(argv[2]) != 1)
    {
        perror("invalid argument type for port number");
        exit(1);
    }

    strcpy(dirname, argv[1]);
    port_number = atoi(argv[2]);
    strcpy(ip_add, argv[3]);

    create_dir(dirname);

    /* create the log file */
    sprintf(log_file, "%s/log_%ld.txt", dirname, (long) getpid());
    int fd = open(log_file, O_WRONLY | O_CREAT, 0666);
    if (fd == -1) 
    {
        perror("file open error (log).");
        exit(1);
    }
    close(fd);

    client();

    return 0;
}
