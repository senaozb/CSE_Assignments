#include "common.h"

struct param
{
    char path1[PATH_LEN];
    char path2[PATH_LEN];
};

sem_t* sem; 

long long int limit_file;

void consumer();

void get_dirname(const char path[PATH_LEN], char name[PATH_LEN])
{
    /* get the name of directory from the path */
    char temp[PATH_LEN];

    strcpy(temp, path);

    char* t = strtok(temp, "/");
    while(t != NULL)
    {
        strcpy(name, t);
        t = strtok(NULL, "/");
    }
}

void close_all(struct shm_entry* entries, size_t size_shm, int shmfd)
{
    /* close shared memory */
    if (munmap(entries, size_shm) == -1) {
        perror("munmap");
        exit(1);
    }
    if (close(shmfd) == -1) {
        perror("close");
        exit(1);
    }
}

void copy_op(void* args)
{
    while(terminate == 0) /* signal loop */
    {
        struct shm_entry* entries;
        int shmfd;
        int num_entry = 0;
        struct param* struct_ptr = (struct param*)args; 

        /* open directories */
        DIR* dir1 = opendir(struct_ptr->path1);
        if(dir1 == NULL) 
        {
            perror("Error opening source directory");
            exit(1);
        }
        DIR* dir2 = opendir(struct_ptr->path2);
        if(dir2 == NULL) 
        {
            if(mkdir(struct_ptr->path2, 0777) == -1) //read, write, execute allowed
            {
                perror("creating directory error");
                exit(1);
            }
            dir2 = opendir(struct_ptr->path2);
            if(dir2 == NULL) 
            {
                perror("Error opening source directory");
                exit(1);
            }
        }

        /* open shared memory  */
        shmfd = shm_open("/buffer", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
        if(shmfd == -1) 
        {
            perror("shm_open error");
            exit(1);
        }

        /* truncate it */
        struct stat mapstat;
        if (-1 != fstat(shmfd, &mapstat) && mapstat.st_size == 0) 
        {
            if (ftruncate(shmfd, size_shm) == -1) 
            {
                perror("ftruncate error");
                exit(1);
            }
        }

        /* map it */
        entries = (struct shm_entry*) mmap(NULL, size_shm, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
        if (entries == MAP_FAILED) {
            perror("Failed to map shared memory object");
            exit(1);
        }
        memset(entries, 0, size_shm);

        struct dirent* item;
        while(terminate == 0) 
        {
            if((num_entry == max_entry) || (num_entry == limit_file))
            {
                /* buffer is full, then call consumer */
                close_all(entries, size_shm, shmfd);
                consumer();
                if (sem_wait(sem) == -1) 
                {    
                    perror("sem_wait failed");
                    exit(1);
                }
                num_entry = 0;

                /* reopen the shared memory and map it */
                shmfd = shm_open("/buffer", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
                if(shmfd == -1) 
                {
                    perror("shm_open error");
                    exit(1);
                }

                entries = (struct shm_entry*) mmap(NULL, size_shm, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
                if (entries == MAP_FAILED) {
                    perror("Failed to map shared memory object");
                    exit(1);
                }

                /* reset the buffer */
                memset(entries, 0, size_shm);
            }


            /* get the item from the directory */
            item = readdir(dir1);
            if(item == NULL)
            {
                close_all(entries, size_shm, shmfd);
                consumer();
                if (sem_wait(sem) == -1) 
                {    
                    perror("sem_wait failed");
                    exit(1);
                }
                terminate = 1;
                break;
            }

            /* skip files with . and .. */
            if(item->d_name[0] == '.') 
                continue;

            /* create paths for the current file */
            char dir1_path[PATH_LEN];
            char dir2_path[PATH_LEN];
            memset(dir1_path, 0, sizeof(dir1_path));
            memset(dir2_path, 0, sizeof(dir2_path));
            memcpy(dir1_path, struct_ptr->path1, sizeof(struct_ptr->path1));
            memcpy(dir2_path, struct_ptr->path2, sizeof(struct_ptr->path2));
            strcat(dir1_path, "/");
            strcat(dir2_path, "/");
            strcat(dir1_path, item->d_name);
            strcat(dir2_path, item->d_name);

            if(item->d_type == 4)
            {
                /* if it is directory, then call the function recursively */
                struct param arg;
                memcpy(arg.path1, dir1_path, sizeof(dir1_path));
                memcpy(arg.path2, dir2_path, sizeof(dir2_path));
            
                /* before calling, sends the buffer to consumer */
                close_all(entries, size_shm, shmfd);
                consumer();
                if (sem_wait(sem) == -1) 
                {    
                    perror("sem_wait failed");
                    exit(1);
                }

                void* new_args = &arg;
        
                /* call */
                copy_op(new_args);
                
                /* reset the number of entries since buffer is empty */
                num_entry = 0;

                /* reopen the shared memory and reset the content */
                shmfd = shm_open("/buffer", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
                if(shmfd == -1) 
                {
                    perror("shm_open error");
                    exit(1);
                }

                entries = (struct shm_entry*) mmap(NULL, size_shm, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
                if (entries == MAP_FAILED) {
                    perror("Failed to map shared memory object");
                    exit(1);
                }

                memset(entries, 0, size_shm);

                /* to keep getting rest of the items */
                if(signal_flag != 1)
                    terminate = 0;
            }
            else 
            {
                if(item->d_type == 1)
                {
                    /* exception for FIFO's */
                    /* create a fifo in destination */
                    int result = mkfifo(dir2_path, 0666); 
                    if (result == -1) {
                        result = unlink(dir2_path);
                        if(result != 0) 
                        {
                            perror("Failed to remove FIFO");
                            exit(1);
                        }
                        result = mkfifo(dir2_path, 0666); 
                        if (result == -1) 
                        {
                            perror("Failed to remove FIFO");
                            exit(1);
                        }
                    }
                    printf("\nFile \"%s\" is created. \n", dir2_path);
                    fflush(stdout);
                    continue;
                } 

                /* open file descriptor for read */
                int fd1 = open(dir1_path, O_RDONLY);
                if (fd1 == -1) {
                    perror("Failed to open file.");
                    exit(1);
                }

                /* open file descriptor for write */
                int fd2 = open(dir2_path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
                if (fd2 == -1) {
                    perror("Failed to open file.");
                    exit(1);
                }
                            
                /* fill the shared memory */
                entries[num_entry].valid = 1;
                entries[num_entry].fd1 = fd1;
                entries[num_entry].fd2 = fd2;
                memcpy(entries[num_entry].path, dir2_path, sizeof(dir2_path)); 

                num_entry++;
            }
        }

        /* close the directories */
        if(closedir(dir1) == -1) 
        {
            perror("Error closing directory 1");
            exit(1);
        }

        if(closedir(dir2) == -1) 
        {
            perror("Error closing directory 2");
            exit(1);
        }
    }
}

void* execute_producer(void* arg)
{
    copy_op(arg); /* call recursive function */

    pthread_exit(NULL);
}

void producer()
{
    struct rlimit lim;

    /* get the soft limit for open files */
    if (getrlimit(RLIMIT_NOFILE, &lim) == -1) {
        perror("Error getting resource limits");
        exit(1);
    }
    limit_file = lim.rlim_cur;

    /* calculate the max entry in the buffer */
    max_entry = buffer_size / (int)sizeof(struct shm_entry);
    /* calculate the size of shared memory */
    size_shm = max_entry * sizeof(struct shm_entry);

    /* check if the destination folder is available */
    /* if not, then create it */
    DIR* dir = opendir(dir2);
    if(dir == NULL) 
    {
        if(mkdir(dir2, 0777) == -1) //read, write, execute allowed
        {
            perror("creating directory error");
            exit(1);
        }
        dir = opendir(dir2);
        if(dir == NULL) 
        {
            perror("Error opening source directory");
            exit(1);
        }
    }
    if(closedir(dir) == -1) 
    {
        perror("Error closing directory");
        exit(1);
    }

    /* add the name of the source folder to the destination path*/
    char dirname[PATH_LEN] = "";
    char new_path[PATH_LEN] = "";
    get_dirname(dir1, dirname);

    memcpy(new_path, dir2, strlen(dir2));
    strcat(new_path, "/");
    strcat(new_path, dirname);

    /* store them in a struct to send to the producer thread */
    struct param path;
    memcpy(path.path1, dir1, PATH_LEN);
    memcpy(path.path2, new_path, PATH_LEN);

    /* producer semaphore */
    sem = sem_open("/producer", O_CREAT, 0644, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open failed");
        exit(1);
    }

    pthread_t th;   
    if(pthread_create(&th, NULL, execute_producer, &path) != 0)
    {
        perror("pthread_create error");
        exit(1);
    }
    if(pthread_join(th, NULL) != 0) 
    {
        perror("Failed to join thread");
        exit(1);
    }

    if(sem_close(sem) == -1)
    {
        perror("sem_close error");
        exit(1);
    }
 
    if(sem_unlink("/producer") == -1)
    {
        perror("unlink error");
        exit(1);
    }

    if(shm_unlink("/buffer") == -1) {
        perror("Failed to unlink shared memory object");
        exit(1);
    }
}