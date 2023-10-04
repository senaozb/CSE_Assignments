#include "fifocommon.h"
#include <libgen.h>

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

int tokenization(char arr[], char tokens[4][50])
{
    int count = 0;
    
    char *token = strtok(arr, " ");

    while(token != NULL) 
    {
        strcpy(tokens[count], token);
        count++;
        if(count == 4)
            break;
        token = strtok(NULL, " ");
    }

    return count;
}

/* list command */
void list_op(char output[])
{
    char buffer[LEN_MES] = "";
    FILE *fp = popen("ls", "r");

    if(fp == NULL) 
    {
        perror("Failed to run command\n");
        exit(1);
    }

    while(fgets(buffer, sizeof(buffer), fp) != NULL) 
    {
        strcat(output, buffer);
    }

    if(pclose(fp) == -1) 
    {
        perror("Error closing the pipe");
        exit(1);
    } 
}

/* readF command */
void readF(char tokens[4][50], char resp[], int count)
{
    FILE *fp = fopen(tokens[1], "r");
    if(fp == NULL) 
    {
        strcpy(resp, "File not found");
        return; 
    }

    int line_num;  
    int current_line = 1;
    char buffer[LEN_MES];
    strcpy(resp, "");

    if(count == 2)
        line_num = -1;
    else 
    {
        if(isValidNumber(tokens[2]) == 1)
            line_num = atoi(tokens[2]); 
        else
        {
            strcpy(resp, "Invalid line number");
            return; 
        }
    }


    while(fgets(buffer, sizeof(buffer), fp) != NULL) 
    {
        if(count == 3)
        {
            if(current_line == line_num) 
            {
                strcpy(resp, buffer);
                break;
            }
            current_line++;
        }
        else
            strcat(resp, buffer);
    }

    if(fclose(fp) == -1) 
    {
        perror("Error close");
        exit(1);
    } 
}

/* writeT command */
void writeT(char tokens[4][50], char resp[], int count)
{
    char temp_file_name[] = "temp.txt";
    char line[500];
    int line_num; 
    int current_line_num = 0;
    int new_flag = 0;
    

    FILE* input_file = fopen(tokens[1], "r");
    FILE* temp_file = fopen(temp_file_name, "w");
    if(input_file == NULL) 
    {
        input_file = fopen(tokens[1], "w+");
        new_flag = 1;
        if(input_file == NULL)
        {
            perror("Failed to open file");
            exit(1);
        }
    }
    if(temp_file == NULL) 
    {
        perror("Failed to open file");
        exit(1);
    }

    if(count == 4)
    {
        if(isValidNumber(tokens[2]) == 1)
            line_num = atoi(tokens[2]); 
        else
        {
            strcpy(resp, "\nInvalid line number");
            return; 
        }
    }
    else
        line_num = -1;
    
    while(fgets(line, 500, input_file)) 
    {
        current_line_num++;
        if(current_line_num == line_num) 
        {
            fputs(tokens[3], temp_file);
            fputs("\n", temp_file);
        } 
        fputs(line, temp_file);
    }
    
    if(count == 3) 
    {   if(new_flag == 0)
            fputs("\n", temp_file);
        fputs(tokens[2], temp_file);
    }
    
    if(fclose(input_file) == -1) 
    {
        perror("Error closing the pipe");
        exit(1);
    } 
    if(fclose(temp_file) == -1) 
    {
        perror("Error closing the pipe");
        exit(1);
    } 
    
    
    if (remove(tokens[1]) != 0) 
    {
        perror("Error removing file");
        exit(1);
    }
    if (rename(temp_file_name, tokens[1]) != 0) 
    {
        perror("Error renaming file");
        exit(1);
    }

    strcpy(resp, "\nThe given string is written to the file");
}

/* upload command */
int upload_op(char tokens[4][50], char path[50])
{
    FILE* stream;
    char full_command[100];

    char new_path[50] = "";
    char *token = strtok(path, "/");
    while(token != NULL) 
    {
        strcat(new_path, "../");
        token = strtok(NULL, "/");
    }

    strcat(new_path, tokens[1]);

    if(access(new_path, F_OK) != 0) 
        return -1;

    sprintf(full_command, "cp %s %s", new_path, "./");
    stream = popen(full_command, "r");
    if(stream == NULL) 
    {
        perror("Error executing command");
        exit(1);
    }       
    if(pclose(stream) == -1) 
    {
        perror("Error closing the pipe");
        exit(1);
    } 

    return 0;
}

/* download command */
int download_op(char tokens[4][50], char path[50])
{
    FILE* stream;
    char full_command[100];

    if(access(tokens[1], F_OK) != 0) 
        return -1;

    char new_path[50] = "";
    char *token = strtok(path, "/");
    while(token != NULL) 
    {
        strcat(new_path, "../");
        token = strtok(NULL, "/");
    }


    sprintf(full_command, "cp %s %s", tokens[1], new_path);
    stream = popen(full_command, "r");
    if(stream == NULL) 
    {
        perror("Error executing command");
        exit(1);
    }       
    if(pclose(stream) == -1) 
    {
        perror("Error closing the pipe");
        exit(1);
    } 

    return 0;
}

/* help command */
void help_op(char command[50], char resp[])
{
    if(strcmp("list", command) == 0)
        strcpy(resp, "list\n -display the list of files in Server's directory\n");
    else if(strcmp("readF", command) == 0)
        strcpy(resp, "readF <file> <line #>\n -display the content of the <file> at the <line#>\n");
    else if(strcmp("writeT", command) == 0)
        strcpy(resp, "writeT <file> <line #> <string (without spaces and \")> \n -write the <string> to the <line #> of the <file>\n");
    else if(strcmp("download", command) == 0)
        strcpy(resp, "download <file>\n -download the <file> in Server's directory\n");
    else if(strcmp("upload", command) == 0)
        strcpy(resp, "upload <file>\n -upload the <file> to Server's directory\n");
    else if(strcmp("quit", command) == 0)
        strcpy(resp, "quit\n -terminate the program\n");
    else if(strcmp("killServer", command) == 0)
        strcpy(resp, "killServer\n -terminate the Server\n");
    else
        strcpy(resp, "invalid command\n");

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
    char text_pid[] = "Client Pid : ";
    int len = snprintf(NULL, 0, "%d", pid);
    char* pid_str = malloc(len+1);
    snprintf(pid_str, len+1, "%d", pid);

    write(fd, text_pid, strlen(text_pid));
    write(fd, pid_str, strlen(pid_str));
    write(fd, "\n", 1);

    /* print the command */
    char text_comm[] = "Command : ";

    write(fd, text_comm, strlen(text_comm));
    write(fd, command, strlen(command));
    write(fd, "\n", 1);

    /* Close the log file and free the allocated mem */
    free(pid_str);      
    if(close(fd) == -1) 
    {
        perror("Error close");
        exit(1);
    } 
}
