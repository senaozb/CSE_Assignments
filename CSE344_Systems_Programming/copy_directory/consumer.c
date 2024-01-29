#include "common.h"

struct shm_entry* entries;
int num_entries = 0;
int finish = 0;
pthread_mutex_t lock_out;
pthread_mutex_t lock_byte;

void* copy_files(void* arg)
{
    struct shm_entry* args = (struct shm_entry*) arg;
    if(args->valid == 1) /* if the entry is not empty */
    {
        /* copy the file */
        while(1)
        {
            char buffer[4096];
            ssize_t bytesRead = read(args->fd1, buffer, sizeof(buffer));
            if (bytesRead == -1) 
            {
                perror("Error reading from file descriptor");
                exit(1);
            }
            if (bytesRead == 0) 
                break;

            ssize_t bytesWritten = write(args->fd2, buffer, bytesRead);
            if (bytesWritten == -1) {
                perror("Error writing to file descriptor");
                exit(1);
            }

            pthread_mutex_lock(&lock_byte);
            num_bytes += bytesWritten;
            pthread_mutex_unlock(&lock_byte);
        }

        if(close(args->fd1) == -1) 
        {
            perror("Error closing file descriptor 1");
            exit(1);
        }

        if(close(args->fd2) == -1) 
        {
            perror("Error closing file descriptor 2");
            exit(1);
        }

        pthread_mutex_lock(&lock_out);
        printf("\nFile \"%s\" is created. \n", args->path);
        pthread_mutex_unlock(&lock_out);
    }

    pthread_exit(NULL);
}

void execute_consumer()
{
    for(int i = 0; i < num_consumers; i++)
    {
        /* when it is done assigning all files to a thread, break the loop*/
        if((num_entries == max_entry) || entries[num_entries].valid == 0)
        {
            finish = 1;
            break;
        }
        else
        {
            /* assign a file to a consumer */
            if(pthread_create(&thread_pool[i], NULL, copy_files, &entries[num_entries]) != 0)
            {
                perror("pthread_create error");
                exit(1);
            }
            if(pthread_join(thread_pool[i], NULL) != 0) 
            {
                perror("Failed to join thread");
                exit(1);
            }
            num_entries++;
        }
    }
}

void consumer()
{
    pthread_mutex_init(&lock_out, NULL);
    pthread_mutex_init(&lock_byte, NULL);

    /* open shared memory */
    int shmfd = shm_open("/buffer", O_RDONLY, S_IRUSR | S_IWUSR);
    if(shmfd == -1) 
    {
        perror("shm_open error");
        exit(1);
    }

    entries = (struct shm_entry*)mmap(NULL, size_shm, PROT_READ, MAP_SHARED, shmfd, 0);
    if (entries == MAP_FAILED) {
        perror("Failed to map shared memory object");
        exit(1);
    }

    while(1)
    {
        /* assign each file to a thread until finish is 1*/
        if(finish == 1)
        {
            /* wake up the producer */
            sem_t* sem = sem_open("/producer", 0);
            if (sem == SEM_FAILED) 
            {
                perror("sem_open failed");
                exit(1);
            }
            if (sem_post(sem) == -1) 
            {
                perror("sem_post failed");
                exit(1);
            }
            finish = 0;
            num_entries = 0;
            break;
        }
        else
            execute_consumer();
    }

    /* close shared memory */
    if (munmap(entries, size_shm) == -1) {
        perror("munmap");
        exit(1);
    }
    if (close(shmfd) == -1) {
        perror("close");
        exit(1);
    }

    pthread_mutex_destroy(&lock_out);
    pthread_mutex_destroy(&lock_byte);
}