#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define MAX_BUFFER 1024

pthread_mutex_t print_lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t print_lock2 = PTHREAD_MUTEX_INITIALIZER;

void* thread_func1(void* param)
{
	pthread_mutex_lock(&print_lock1);
	pthread_mutex_lock(&print_lock2);
	printf("thread1\n");

	char buffer[MAX_BUFFER];
	int thread_socket_fd;
	struct sockaddr_in sock1_add, sock2_add;
	int ln;

	char message[MAX_BUFFER] = "Message from thread 1";

	thread_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(thread_socket_fd < 0)
	{
		perror("socket error");
		pthread_exit(NULL);
	}
	printf("thread1 socket\n");

	memset(&sock1_add, 0, sizeof(sock1_add));
	memset(&sock2_add, 0, sizeof(sock2_add));

	sock1_add.sin_family      = AF_INET;
	sock1_add.sin_port        = 8000;
	sock1_add.sin_addr.s_addr = inet_addr("127.0.0.1");

	printf("thread1 configured\n");

	if(bind(thread_socket_fd, (struct sockaddr *) &sock1_add, sizeof(sock1_add)) != 0)
	{
		printf("thread1 exit\n");
		perror("bind error");
		pthread_exit(NULL);
	}

	printf("thread1 binded\n");

	pthread_mutex_unlock(&print_lock2);
	pthread_mutex_unlock(&print_lock1);


	pthread_mutex_lock(&print_lock2);
	pthread_mutex_lock(&print_lock1);


	printf("ikinci lock\n");
	socklen_t len1 = sizeof(sock1_add);
	socklen_t len2 = sizeof(sock2_add);
	ln = recvfrom(thread_socket_fd, buffer, MAX_BUFFER, 0, (struct sockaddr *) &sock2_add, &len2);
	if(ln < 0)
	{
		perror("recvfrom error");
		pthread_exit(NULL);
	}
	buffer[ln] = '\0';

	printf("thread1 received\n");
	printf("t1 received : %s\n", buffer);

	if(sendto(thread_socket_fd, message, MAX_BUFFER, 0, (struct sockaddr *) &sock2_add, len2) < 0)
	{
		perror("sendto error");
		pthread_exit(NULL);
	}

	if(close(thread_socket_fd) != 0)
	{
		perror("close error");
		pthread_exit(NULL);
	}
	printf("thread1 closed\n");
	pthread_mutex_unlock(&print_lock1);

	pthread_mutex_unlock(&print_lock2);

}
void* thread_func2(void* param)
{
	pthread_mutex_lock(&print_lock2);
	pthread_mutex_lock(&print_lock1);

	printf("thread2\n");

	char buffer[MAX_BUFFER];
	int thread_socket_fd;
	struct sockaddr_in sock1_add;
	int ln;

	char message[MAX_BUFFER] = "Message from thread 2";

	thread_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(thread_socket_fd < 0)
	{
		perror("socket error");
		pthread_exit(NULL);
	}
	printf("thread2 socket\n");

	memset(&sock1_add, 0, sizeof(sock1_add));

	sock1_add.sin_family      = AF_INET;
	sock1_add.sin_port        = 8000;
	sock1_add.sin_addr.s_addr = inet_addr("127.0.0.1");

	printf("thread2 configured\n");


	if(sendto(thread_socket_fd, message, MAX_BUFFER, 0, (struct sockaddr *) &sock1_add, sizeof(sock1_add)) < 0)
	{
		perror("sendto error\n");
		pthread_exit(NULL);
	}

	printf("thread2 sent\n");

	pthread_mutex_unlock(&print_lock1);
	pthread_mutex_unlock(&print_lock2);

	socklen_t len1 = sizeof(sock1_add);
	ln = recvfrom(thread_socket_fd, buffer, MAX_BUFFER, 0, (struct sockaddr *) &sock1_add, &len1);
	if(ln < 0)
	{
		perror("recvfrom error");
		pthread_exit(NULL);
	}
	buffer[ln] = '\0';

	printf("thread2 received\n");
	printf("t2 received : %s\n", buffer);
	if(close(thread_socket_fd) != 0)
	{
		perror("close error");
		pthread_exit(NULL);
	}
	printf("thread2 closed\n");
}

int main(void)
{
    pthread_t t1, t2;

    if(pthread_mutex_init(&print_lock1, NULL) != 0)
    {
		perror("init error");
		exit(1);
	}
    if(pthread_mutex_init(&print_lock2, NULL) != 0)
    {
		perror("init error");
		exit(1);
	}

    if(pthread_create(&t1, NULL, thread_func1, NULL) != 0)
    {
		perror("create error");
		exit(1);
	}
    if(pthread_create(&t2, NULL, thread_func2, NULL) != 0)
    {
		perror("create error");
		exit(1);
	}

    if(pthread_join(t1, NULL) != 0)
    {
    	perror("join error");
    	exit(1);
    }
    if(pthread_join(t2, NULL) != 0)
    {
    	perror("join error");
    	exit(1);
    }

    if(pthread_mutex_destroy(&print_lock1) != 0)
    {
		perror("destroy error");
		exit(1);
	}
    if(pthread_mutex_destroy(&print_lock2) != 0)
    {
		perror("destroy error");
		exit(1);
	}

    return 0;
}
