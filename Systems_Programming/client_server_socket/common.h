#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <dirent.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/wait.h>

#define DEFAULT_SIZE 300 /* path name size */
#define BUFFER_SIZE 1024 /* buffer size for socket messages */
#define MAX_MESSAGE 100 /* messages that can be sent in one interval */

struct file_info /* file information struct */
{
    char path[DEFAULT_SIZE];
    time_t modified_time;
};

struct dir_state /* directory state struct */
{
    struct file_info* files;
    int num_files;
};

/* socket message meanings */
//A1 = ADD
//D2 = DELETE
//M3 = MODIFY
//EXIT = EXIT
//END = FINISH OPERATION
//FULL = SERVER FULL
//GO = SERVER NOT FULL
