#include "common.h"

/* thrad parameters */
struct param
{
    int fd;
    int index;
};

char dirname[DEFAULT_SIZE]; /* server directory path */
int pool_size; 
int port_number;
int server_fd, client_fd;
struct sockaddr_in server_address, client_address;
socklen_t client_address_length;

pthread_mutex_t num_client_mutex;

/* thread pool */
pthread_t *pool;
struct param *clients;
int num_clients = 0;

int terminate = 0;

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

void send_exit_message()
{
    for(int i = 0; i < pool_size; i++)
    {
        if(clients[i].fd != -1)
        {
            int bytes_sent = send(clients[i].fd, "EXIT", BUFFER_SIZE, 0);
            if (bytes_sent == -1) 
            {
                perror("send failed");
                exit(1);
            }

            int result = close(clients[i].fd);
            if (result == -1) 
            {
                perror("Failed to close file descriptor");
                exit(1);
            }
        }
    }
}

/* add client to the clients array */
int add_client(int fd)
{
    for(int i = 0; i < pool_size; i++)
    {
        if(clients[i].fd == -1)
        {
            clients[i].fd = fd;
            return i;
        }
    }
    return -1;
}

void add_file_message(char filename[], int fd)
{
    char arr[BUFFER_SIZE];
    sprintf(arr, "A1/%s", filename);

    int bytes_sent = send(fd, arr, BUFFER_SIZE, 0);
    if (bytes_sent == -1) 
    {
        perror("send failed");
        exit(1);
    }
}

void delete_file_message(char filename[], int fd)
{
    char arr[BUFFER_SIZE];
    sprintf(arr, "D2/%s", filename);

    int bytes_sent = send(fd, arr, BUFFER_SIZE, 0);
    if (bytes_sent == -1) 
    {
        perror("send failed");
        exit(1);
    }
}

void modify_file_message(char filename[], int fd)
{
    char arr[BUFFER_SIZE];
    sprintf(arr, "M3/%s", filename);

    int bytes_sent = send(fd, arr, BUFFER_SIZE, 0);
    if (bytes_sent == -1) 
    {
        perror("send failed");
        exit(1);
    }
}

int read_from_client(int fd, char messages[][1024], int *count)
{
    int flag = 0;
    char temp_file[DEFAULT_SIZE];
    FILE* file;

    while(terminate == 0) /* check if a signal received */
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
            {
                int result = fclose(file);
                if(result != 0) 
                {
                    perror("Failed to close file");
                    exit(1);
                }
            }
            return 0;
        }
        if(strcmp("EXIT", arr) == 0) /* client exits, remove it */
        {
            if(flag == 1)
            {
                int result = fclose(file);
                if(result != 0) 
                {
                    perror("Failed to close file");
                    exit(1);
                }
            }
            return 1;
        }
        if(strncmp(arr, "A1|", 3) == 0 || strncmp(arr, "M3|", 3) == 0) /* file is added or modified */
        {
            strcpy(temp_file, dirname);
            const char* delimiter = "|";

            /* get the path */
            char* sub = strtok(arr, delimiter);
            sub = strtok(NULL, delimiter);
            
            char* token = NULL;
            char* result = NULL;

            /* change the beginning of the path with server's directory path */
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

            /* add the message to temp message array */
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
        else if(strncmp(arr, "D2|", 3) == 0) /* file is deleted */
        {
            strcpy(temp_file, dirname);
            const char* delimiter = "|";

            char* sub = strtok(arr, delimiter);
            sub = strtok(NULL, delimiter);
            
            char* token = NULL;
            char* result = NULL;

            /* change the beginning of the path with server's directory path */
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
        else if(flag == 1)  /* write the content of the file */
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

void send_from_server(char arr[], int fd, char messages[][1024], int count, char filename_sent[DEFAULT_SIZE])
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

    if(strncmp(arr, "A1|", 3) == 0 || strncmp(arr, "M3|", 3) == 0) /* if a file is added or modified */
    {
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
        if (entry->d_name[0] == '.') /* skip the files starting with . */
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
                    strcpy(arr, "M3|");
                    strcat(arr, previous.files[i].path);
                    send_from_server(arr, fd, messages, count, previous.files[i].path);
                }
                break;
            }
        }
        if (found == 0) /* file is deleted */
        {
            strcpy(arr, "D2|");
            strcat(arr, previous.files[i].path);
            send_from_server(arr, fd, messages, count, previous.files[i].path);
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
            send_from_server(arr, fd, messages, count, current.files[i].path);
        }
    }

    /* the message is ended with END */
    int bytes_sent = send(fd, "END", BUFFER_SIZE, 0); 
    if (bytes_sent == -1) 
    {
        perror("send failed");
        exit(1);
    }

    free(previous.files);
}

void sync_dir(int fd)
{
    /* send the current directory to client */
    struct dir_state dir =  get_dir_state(dirname);
    int count = 0;
    char messages[MAX_MESSAGE][1024];

    for (int i = 0; i < dir.num_files; i++) 
    {
        char arr[BUFFER_SIZE] = "";

        strcpy(arr, "A1|");
        strcat(arr, dir.files[i].path);
        send_from_server(arr, fd, messages, count, dir.files[i].path);
    }

    /* the message is ended with END */
    int bytes_sent = send(fd, "END", BUFFER_SIZE, 0); 
    if (bytes_sent == -1) 
    {
        perror("send failed");
        exit(1);
    }
}

void perform_client_op(int fd)
{
    struct dir_state previous, current;
    previous = get_dir_state(dirname);
    char messages[MAX_MESSAGE][1024];
    int count = 0;

    while(terminate == 0)
    {
        current = get_dir_state(dirname); /* get the current directory state */
        check_dir(previous, current, fd, messages, count); /* compare them */
        previous = current; /* update previous */

        if(terminate == 1) /* if a signal is caught, break the loop before sleeping */
            break;

        sleep(5);

        memset(messages, 0, sizeof(messages));
        count = 0;
        int exit_f = read_from_client(fd, messages, &count); /* get messages from client */
        if(exit_f == 1) /* client is terminated, remove the client */
            return;
    }

    int result = close(fd);
    if (result == -1) 
    {
        perror("Failed to close file descriptor");
        exit(1);
    }
}

void* thread_function(void* arg)
{
    pthread_detach(pthread_self());
    
    /* signals for this thread */
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction error");
        exit(1);
    }

    int new_fd;

    struct param obj = *(struct param*)arg;
    new_fd = obj.fd;

    
    perform_client_op(new_fd);

    if(pthread_mutex_lock(&num_client_mutex) != 0) 
    {
        perror("Mutex lock failed");
        pthread_exit(NULL);
    }

    clients[obj.index].fd = -1;
    num_clients--;

    printf("A client is disconnected.\n");
    fflush(stdout);

    if (pthread_mutex_unlock(&num_client_mutex) != 0) 
    {
        perror("Mutex unlock failed");
        pthread_exit(NULL);
    }

    pthread_exit(NULL);
}


void server() 
{
    /* create server socket */
    memset(&server_address, 0 ,sizeof(struct sockaddr_in));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_number);
    server_address.sin_addr.s_addr = INADDR_ANY;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == -1) 
    {
        perror("Socket creation failed");
        exit(1);
    }

    if(bind(server_fd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) 
    {
        perror("Bind failed");
        exit(1);
    }

    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == -1) {
        perror("Error getting hostname");
        exit(EXIT_FAILURE);
    }

    struct hostent* host;
    if ((host = gethostbyname(hostname)) == NULL) {
        perror("Error getting host by name");
        exit(EXIT_FAILURE);
    }

    struct in_addr** addr_list = (struct in_addr**)host->h_addr_list;
    for (int i = 0; addr_list[i] != NULL; ++i) {
        printf("IP Address: %s\n", inet_ntoa(*addr_list[i]));
        fflush(stdout);
    }    

    if(listen(server_fd, pool_size) == -1) 
    {
        perror("Listen failed");
        exit(1);
    }

    printf("Server listening on port %d\n", port_number);
    fflush(stdout);

    while(terminate == 0) 
    {
        client_address_length = sizeof(client_address); // ctrl -c problem accept
        if ((client_fd = accept(server_fd, (struct sockaddr*)&client_address, &client_address_length)) == -1) 
        {
            if(terminate == 1)
            {
                printf("Server is terminated!\n");
                fflush(stdout);
                break;
            }
            else
            {
                perror("Accept failed");
                exit(1);
            }
        }

        if(num_clients < pool_size)
        {
            char ans[BUFFER_SIZE] = "GO";
            int bytes_sent = send(client_fd, ans, BUFFER_SIZE, 0);
            if (bytes_sent == -1) 
            {
                perror("send failed");
                exit(1);
            }

            /* add the client */
            int ind = add_client(client_fd);
            if(ind == -1)
            {
                perror("add client error");
                exit(1);
            }

            printf("A client is connected\n");
            fflush(stdout);

            num_clients++;

            sync_dir(client_fd);

            /* start execution */
            if(pthread_create(&pool[ind], NULL, thread_function, &clients[ind]) != 0)
            {
                perror("pthread create error");
                exit(1);
            }
        }
        else 
        {
            char ans[BUFFER_SIZE] = "FULL";
            int bytes_sent = send(client_fd, ans, BUFFER_SIZE, 0);
            if (bytes_sent == -1) 
            {
                perror("send failed");
                exit(1);
            }
        }
    }
    
    if(num_clients != 0)
        send_exit_message();

    close(server_fd);
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

    if(isValidNumber(argv[2]) != 1 || isValidNumber(argv[3]) != 1)
    {
        perror("invalid argument type");
        exit(1);
    }

    strcpy(dirname, argv[1]);
    pool_size = atoi(argv[2]);
    port_number = atoi(argv[3]);

    pool = (pthread_t*)calloc(pool_size, sizeof(pthread_t));
    if(pool == NULL) 
    {
        perror("calloc error");
        exit(1);
    }
    clients = (struct param*)calloc(pool_size, sizeof(struct param));
    if(clients == NULL) 
    {
        perror("calloc error");
        exit(1);
    }

    for(int i = 0; i < pool_size; i++) 
    {
        clients[i].fd = -1;
        clients[i].index = i;
    }
    

    if(pthread_mutex_init(&num_client_mutex, NULL) != 0) 
    {
        perror("mutex init error");
        exit(1);
    }

    server();

    if(pthread_mutex_destroy(&num_client_mutex) != 0)
    {
        perror("mutex destroy error");
        exit(1);
    }

    free(pool);
    free(clients);

    return 0;
}