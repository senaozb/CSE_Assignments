#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sched.h>


#define MAX_BUFFER 1024

void* server_thread(void* param)
{

	char buff[MAX_BUFFER];
	char message[MAX_BUFFER] = "message from server";
	int thread_socket_fd, client_fd;
	struct sockaddr_in sock_ser_add, sock_cli_add;

	thread_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(thread_socket_fd < 0)
	{
		perror("socket error");
		pthread_exit(NULL);
	}
	printf("server socket\n");



	memset(&sock_ser_add, 0, sizeof(sock_ser_add));
	memset(&sock_cli_add, 0, sizeof(sock_cli_add));

	sock_ser_add.sin_family      = AF_INET;
	sock_ser_add.sin_port        = 8000;
	sock_ser_add.sin_addr.s_addr = inet_addr("127.0.0.1");
	printf("server configured\n");



	if(bind(thread_socket_fd, (struct sockaddr *) &sock_ser_add, sizeof(sock_ser_add)) != 0)
	{
		perror("bind error");
		pthread_exit(NULL);
	}

	printf("server binded\n");



	if ((listen(thread_socket_fd, 1)) != 0)
	{
		perror("listen error");
		pthread_exit(NULL);
	}
	else
		printf("Server listening..\n");

	socklen_t len = sizeof(sock_cli_add);
	client_fd = accept(thread_socket_fd, (struct sockaddr *) &sock_cli_add, &len);
	if (client_fd < 0)
	{
		perror("accept error");
		pthread_exit(NULL);
	}

	printf("server accept the client...\n");


	int ln = recv(client_fd, buff, sizeof(buff), 0);
	buff[ln] = '\0';
	printf("server received: %s\n", buff);

	send(client_fd, message, sizeof(message), 0);
	printf("server writes '%s' \n", message);


    if(close(thread_socket_fd) != 0)
	{
		perror("close error");
		pthread_exit(NULL);
	}
	printf("server closed\n");

}
void* client_thread(void* param)
{

	char buff[MAX_BUFFER];
	char message[MAX_BUFFER] = "message from client";
	int thread_socket_fd;
	struct sockaddr_in sock_ser_add;

	thread_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(thread_socket_fd < 0)
	{
		perror("socket error");
		pthread_exit(NULL);
	}
	printf("client socket\n");

	memset(&sock_ser_add, 0, sizeof(sock_ser_add));

	sock_ser_add.sin_family      = AF_INET;
	sock_ser_add.sin_port        = 8000;
	sock_ser_add.sin_addr.s_addr = inet_addr("127.0.0.1");

	printf("client configured\n");

	if (connect(thread_socket_fd, (struct sockaddr *) &sock_ser_add, sizeof(sock_ser_add)) != 0)
	{
		perror("connect error");
		pthread_exit(NULL);
	}
	else
		printf("connected to the server..\n");


	send(thread_socket_fd, message, sizeof(message), 0);
	printf("client writes '%s' \n", message);


	int ln = recv(thread_socket_fd, buff, sizeof(buff), 0);
	buff[ln] = '\0';

	printf("client received: %s\n", buff);

	if(close(thread_socket_fd) != 0)
	{
		perror("close error");
		pthread_exit(NULL);
	}
	printf("client closed\n");

}


int main(void)
{
	pthread_t server, client;
	pthread_attr_t attr;
	struct sched_param param_main;
	int policy = SCHED_FIFO;

	printf("program is started \n");

    /* Set processor affinity */
    unsigned long mask = 1; /* use only 1 CPU core */
    unsigned int len = sizeof(mask);
    int status = sched_setaffinity(0, len, &mask);
    if (status < 0) perror("sched_setaffinity");
    status = sched_getaffinity(0, len, &mask);
    if (status < 0) perror("sched_getaffinity");

	if(pthread_attr_init (&attr) != 0)
	{
		perror("init error");
		exit(1);
	}
	if(pthread_create(&server, &attr, server_thread, NULL) != 0)
	{
		perror("create error");
		exit(1);
	}
	param_main.sched_priority = 200;
	pthread_setschedparam(server, policy, &param_main);
	if(pthread_attr_destroy(&attr) != 0)
	{
		perror("destroy error");
		exit(1);
	}



	if(pthread_attr_init (&attr) != 0)
	{
		perror("init error");
		exit(1);
	}
	if(pthread_create(&client, &attr, client_thread, NULL) != 0)
	{
		perror("create error");
		exit(1);
	}
	param_main.sched_priority = 100;
	pthread_setschedparam(client, policy, &param_main);
	if(pthread_attr_destroy(&attr) != 0)
	{
		perror("destroy error");
		exit(1);
	}

	if(pthread_join(server, NULL) != 0)
	{
		perror("join error");
		exit(1);
	}
	if(pthread_join(client, NULL) != 0)
	{
		perror("join error");
		exit(1);
	}

    return 0;
}
