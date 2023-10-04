#include "common.h"

size_t size_shm = 0;
int max_entry = 0;
int terminate = 0;
long int num_bytes = 0;
int signal_flag = 0;

pthread_t* thread_pool;

int buffer_size;
int num_consumers;
const char* dir1;
const char* dir2;

int producer();

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

/* signal handler */
void handler(int signum)
{
    switch(signum)
    {
        case SIGINT:
            terminate = 1;
            signal_flag = 1;
            printf("\nSIGINT is received. Terminating!\n");
            fflush(stdout);
            break;
        case SIGTERM:
            terminate = 1;
            signal_flag = 1;
            printf("\nSIGTERM is received. Terminating!\n");
            fflush(stdout);
            break;
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

    /* argument assignment */
    if(argc != 5)
    {
        perror("Invalid argument number");
        exit(1);
    }
    if(isValidNumber(argv[1]) == 1 && isValidNumber(argv[2]) == 1)
    {
        buffer_size = atoi(argv[1]);
        if(buffer_size < sizeof(struct shm_entry))
        {
            fprintf(stderr, "Buffer size should be greater than or equal to %ld\n", sizeof(struct shm_entry));
            exit(1);
        }
        num_consumers = atoi(argv[2]);
        if(num_consumers < 1)
        {
            fprintf(stderr, "Number of consumers should be greater than 0\n");
            exit(1);
        }
        dir1 = argv[3];
        dir2 = argv[4];
    }
    else
    {
        perror("Invalid argument type");
        exit(1);
    }

    /* thread pool creation */
    thread_pool = (pthread_t*)calloc(num_consumers, sizeof(pthread_t));
    if(thread_pool == NULL) 
    {
        perror("calloc error");
        exit(1);
    }

    /* get the current time */
    struct timeval start_time;
    gettimeofday(&start_time, NULL);

    /* start the program */
    producer();

    /* get the current time */
    struct timeval end_time;
    gettimeofday(&end_time, NULL);

    long spent_time = 1000000*(end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec);

    printf("\n\nTotal time passed in microseconds : %ld\n", spent_time);
    fflush(stdout);
    printf("\nTotal %ld bytes are transferred\n\n", num_bytes);
    fflush(stdout);

    free(thread_pool);
    return 0;
}
