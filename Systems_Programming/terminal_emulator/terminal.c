#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

pid_t pid; /* current pid */
int def_stdin, def_stdout; /* original, default stdin and stdout */

void handler(int signum)
{
    switch(signum)
    {
        case SIGINT:
            puts("\nSIGINT is received.\n");
            kill(pid, SIGTERM); /* kill the current child process */
            break;
        case SIGTERM:
            puts("\nSIGTERM is received.\n");
            break;
    }
}

void log_print(char filename[], pid_t pid, char command[])
{
    /* open the log file */
    int fd = open(filename, O_WRONLY | O_APPEND);
    if (fd == -1) {
        perror("Failed to open log file.");
        exit(1);
    }

    /* convert pid number to string */
    char text_pid[] = "Pid : ";
    int len = snprintf(NULL, 0, "%d", pid);
    char* pid_str = malloc(len+1);
    snprintf(pid_str, len+1, "%d", pid);

    write(fd, text_pid, strlen(text_pid));
    write(fd, pid_str, strlen(pid_str));
    write(fd, "\n", 1);

    /*print the command */
    char text_comm[] = "Command : ";

    write(fd, text_comm, strlen(text_comm));
    write(fd, command, strlen(command));
    write(fd, "\n", 1);

    /* Close the log file and free the allocated mem */
    free(pid_str);
    close(fd);
}

void read_input(char input[])
{
    printf("\n$ ");
    input = fgets(input, 2500, stdin);
    int len = strlen(input);
    input[len-1] = '\0'; /* remove the newline */
}

int split_commands(char commands_pipes[20][100], char input[])
{
    /* split the commands when a pipe exists */
    int count = 0;

    char* t = strtok(input, "|");
    while(t != NULL)
    {
        strcpy(commands_pipes[count], t);
        count++;
        t = strtok(NULL, "|");
    }

    return count;
}

int is_redirection(char command[], char splitted_command[2][100])
{
    int input = 0;
    int output = 0;
    int flag = 0, count = 0;
    char temp[100];
    
    /* check if it is < */
    strcpy(temp, command);
    char* t = strtok(temp, "<");
    while(t != NULL)
    {
        input++;
        t = strtok(NULL, "<");
    }
    input--;

    /* check if it is > */
    strcpy(temp, command);
    char* m = strtok(temp, ">");
    while(m != NULL)
    {
        output++;
        m = strtok(NULL, ">");
    }
    output--;

    /* split the command if there is a redirection */
    strcpy(temp, command);
    if(input != 0)
    {
        flag = 1;
        char* l = strtok(temp, "<");
        while(l != NULL)
        {
            strcpy(splitted_command[count], l);
            count++;
            l = strtok(NULL, "<");
        }
    }
    else if(output != 0)
    {
        flag = 2;
        char* l = strtok(temp, ">");
        while(l != NULL)
        {
            strcpy(splitted_command[count], l);
            count++;
            l = strtok(NULL, ">");
        }
    }
    else
        flag = 0;

    return flag; /* return an indicator */
}

void remove_space(char str[]) 
{
    char temp_str[100];
    int len = strlen(str);

    int j = 0;
    for (int i = 0; i < len; i++) 
    {
        if (str[i] != ' ')
        {
            temp_str[j] = str[i];
            j++;
        }
    }
    temp_str[j] = '\0';

    strcpy(str, temp_str);
}


int execute(char command[], int prev_fd, int i, int count, char filename[])
{
    pid_t f;
    char splitted_command[2][100];
    char temp[100];

    strcpy(temp, command);

    int flag = is_redirection(command, splitted_command);

    /* if redirection < is in the command */
    if(flag == 1)
    {
        strcpy(command, splitted_command[0]); /* copy only the command */
        
        remove_space(splitted_command[1]); /* remove space to get the filename */
        int input_fd = open(splitted_command[1], O_RDONLY, 0666);
        if (input_fd == -1) 
        {
            perror("file open error.");
            exit(1);
        }

        /* change the input source */
        if(dup2(input_fd, STDIN_FILENO) == -1)
        {
            perror("dup2 error");
            exit(1);
        }
        if(close(input_fd) == -1)
        {
            perror("file close error");
            exit(1);
        }

    }
    else if(flag == 2) /* if redirection > is in the command */
    {
        strcpy(command, splitted_command[0]); /* copy only the command */

        remove_space(splitted_command[1]); /* remove space to get the filename */
        int output_fd = open(splitted_command[1], O_WRONLY | O_CREAT, 0666);
        if (output_fd == -1) 
        {
            perror("file open error.");
            exit(1);
        }

        /* change the output source */
        if(dup2(output_fd, STDOUT_FILENO) == -1)
        {
            perror("dup2 error");
            exit(1);
        }
        if(close(output_fd) == -1)
        {
            perror("file close error");
            exit(1);
        }

    }


    /* if the command is not the last one */
    if(i != count-1) 
    {
        /* create a pipe*/
        int pfd[2];

        if (pipe(pfd) < 0)
        {
            perror("pipe failed");
            if(close(prev_fd) == -1)
            {
                perror("file close error");
                exit(1);
            }
            exit(1);
        }

        /* create a child process using fork */
        f = fork();
        if (f > 0) /* parent process */
        {
            pid = f;
            log_print(filename, f, temp);
           
            /* we don't need prev_fd and write end of this pipe anymore so close them */
            if(close(prev_fd) == -1)
            {
                perror("file close error");
                exit(1);
            }
            if(close(pfd[1]) == -1)
            {
                perror("file close error");
                exit(1);
            }
            /* wait for child process */
            int status;
            pid_t child_pid;

            do 
            {
                child_pid = wait(&status);
            } while (child_pid == -1 && errno == EINTR);

        }
        else if (f == 0) /* child process */
        {
            /* duplicate stdin with previous fd to read and then close prev_fd*/
            if(flag != 1)
            {
                if(dup2(prev_fd, STDIN_FILENO) == -1)
                {
                    perror("dup2 error");
                    exit(1);
                }
            }
            if(close(prev_fd) == -1)
            {
                perror("file close error");
                exit(1);
            }

            /* duplicate stdout with the current pipe's write end and then close write end*/
            if(flag != 2)
            {
                if(dup2(pfd[1], STDOUT_FILENO) == -1)
                {
                    perror("dup2 error");
                    exit(1);
                }
            }
                if(close(pfd[1]) == -1)
                {
                    perror("file close error");
                    exit(1);
                }

            /* child needs to read from read end of the previous pipe
               which is stored by prev_fd so we don't need to read end of this pipe */
            if(close(pfd[0]) == -1)
            {
                perror("file close error");
                exit(1);
            }
        
            /* execute the command */
            execl("/bin/sh", "sh", "-c", command, (char *)NULL);
            perror("execl failed");
            exit(1);
        }
        else /* fork failed, then close all fd's */
        {
            perror("fork failed");
            if(close(pfd[0]) == -1)
            {
                perror("file close error");
                exit(1);
            }
            if(close(pfd[1]) == -1)
            {
                perror("file close error");
                exit(1);
            }
            if(close(prev_fd) == -1)
            {
                perror("file close error");
                exit(1);
            }
            exit(1);
        }

        return pfd[0]; //return the read end
    }
    /* if the command is the last one, then do not create a pipe */
    else
    {
        /* create a child process using fork */
        f = fork();
        if (f > 0) /* parent process */
        {
            pid = f;
            log_print(filename, f, temp);
            /* we don't need prev_fd */
            if(close(prev_fd) == -1)
            {
                perror("file close error");
                exit(1);
            }
            /* wait for child process */
            int status;
            pid_t child_pid;

            do 
            {
                child_pid = wait(&status);
            } while (child_pid == -1 && errno == EINTR);

        }
        else if (f == 0) /* child process */
        {
            /* duplicate stdin with previous fd to read and close prev_fd*/
            if(flag != 1)
            {
                if(dup2(prev_fd, STDIN_FILENO) == -1)
                {
                    perror("dup2 error");
                    exit(1);
                }
            }
            if(close(prev_fd) == -1)
            {
                perror("file close error");
                exit(1);
            }

            /*we don't need to change stdout since there is no command left*/

            /* execute the command */
            execl("/bin/sh", "sh", "-c", command, (char *)NULL);
            perror("execl failed");
            exit(1);
        }
        else /* fork failed, then close all fd's */
        {
            perror("fork failed");
            if(close(prev_fd) == -1)
            {
                perror("file close error");
                exit(1);
            }
            exit(1);
        }

        return -1;
    }

}

int main(int argc, char *argv[]) 
{
    char input_str[2500];
    char commands_pipes[20][100];
    int count = 0; /* #commands */
    int prev_fd = 0; /* to store the read end of the previous pipe */

    /* log file variables */
    char filename[30];
    char time_str[20];
    
    /* Signals */
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

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
    
    /* store the original stdin and stdout */
    def_stdin = dup(STDIN_FILENO);
    def_stdout = dup(STDOUT_FILENO);
    

    printf("\nWelcome to the terminal.\n\n");

    while(1) 
    {
        /* Get time and convert it to string and create a file using this timestamp */
        time_t t = time(NULL);
        strftime(time_str, sizeof(time_str), "%Y%m%d_%H%M%S", localtime(&t));
        sprintf(filename, "log_%s.txt", time_str);
        int fd = open(filename, O_WRONLY | O_CREAT, 0666);
        if (fd == -1) 
        {
            perror("file open error (log).");
            exit(1);
        }
        close(fd);

        /* convert sdtin and stdout to default versions*/
        if(dup2(def_stdin, STDIN_FILENO) == -1)
        {
            perror("dup2 error");
            exit(1);
        }
        if(dup2(def_stdout, STDOUT_FILENO) == -1)
        {
            perror("dup2 error");
            exit(1);
        }

        /* The previous read end is now stdin so duplicate it since this is the first command */
        prev_fd = dup(def_stdin);
        if(prev_fd == -1)
        {
            perror("dup error");
            exit(1);
        }

        /* get the input */
        read_input(input_str);

        /* split them */
        count = split_commands(commands_pipes, input_str);

        /* Execute each command one by one */
        for(int i = 0; i < count; i++)
        {
            if(strcmp(commands_pipes[i], ":q") == 0)
            {
                if(close(prev_fd) == -1)
                {
                    perror("file close error");
                    exit(1);
                }
                exit(1);
            }
            prev_fd = execute(commands_pipes[i], prev_fd, i, count, filename);   
        }
    }

    return 1;
    
}

