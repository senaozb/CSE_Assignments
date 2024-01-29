#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/time.h>
#include <semaphore.h>
#include <signal.h>
#include <ctype.h>
#include <sys/resource.h>


#define PATH_LEN 300

struct shm_entry
{
    int fd1; // read fd
    int fd2; // write fd
    char path[PATH_LEN]; // the exact path
    int valid; // flag,  1 when it is not empty, 0 when it is empty
};

extern int terminate; // terminate flag (mostly for signals)
extern int signal_flag;

extern long int num_bytes;

extern int buffer_size; // buffer size as an argument 
extern int num_consumers; // number of consumers as an argument
extern const char* dir1; // the source path
extern const char* dir2; // the destination path

extern int max_entry; // the max entry that buffer can hold
extern size_t size_shm; // the size of the shared memory using max_entry
extern pthread_t* thread_pool; // thread pool 
