/*
 * serverpere.cpp
 *
 *  Created on: 16 janv. 2012
 *      Author: remi
 */

#include "server_pere.h"
#include <iostream>
using namespace std;
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
bool server_pere::s_opened=true;



server_pere::~server_pere() {

}



server_pere::server_pere(int sensorServerBox,int actuatorServerBox) :
		p_sensorServerBox(sensorServerBox), p_actuatorServerBox(actuatorServerBox)  {
		open_thread_comm_client();
}




void *server_pere::createCommClient_2(void * ptr)
{
	server_pere* p = (server_pere*)ptr;
	p->createCommClient();
	return (0);
}

void *server_pere::createCommClient()
{
	cout << "Create comm client : Socket : " << p_fd << endl;
	communication_client comm_client(p_sensorServerBox, p_actuatorServerBox, p_fd, sockfd);
	return (0);
}

void *server_pere::open_socket_2(void * ptr)
{
	server_pere* p = (server_pere*)ptr;
	p->open_socket();
	return (0);
}

void *server_pere::open_socket()
{
	cout << "Open Socket" << endl;
	unsigned int size;
	struct sockaddr_in local;
	struct sockaddr_in remote;

	bzero(&local, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_port = htons(3012);
	local.sin_addr.s_addr = INADDR_ANY;
	bzero(&(local.sin_zero), 8);

	/*if((sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
	perror("socket");
	exit(1);
	}*/


	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	int on = 1;
	int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	ret = bind(sockfd, (struct sockaddr *)&local, sizeof(sockaddr));


	/*if(bind(sockfd, (struct sockaddr *)&local, sizeof(struct sockaddr)) == -1)
	{
	perror("bind");
	exit(1);
	}*/
	if(listen(sockfd, 5) == -1)
	{
	perror("listen");
	exit(1);
	}
	size = sizeof(struct sockaddr_in);


	while(s_opened)
	{
	p_fd = accept(sockfd, (struct sockaddr *)&remote, &size);
	if((p_fd>0)&(s_opened))
	{
			cout << "File descriptor" << endl;
			cout << p_fd << endl ;
			cout << s_opened << endl;
			int reussite;
			pthread_t thread_sock;
			reussite = pthread_create(&thread_sock, NULL, &server_pere::createCommClient_2, this);
	}

	}
	cout << "Sortie du while" << endl;
	close(p_fd);
	shutdown(sockfd,SHUT_RDWR);
	pthread_exit(0);

	return(0);
}



int server_pere::open_thread_comm_client()
{

	int check = pthread_create(&thread_comm_client, NULL, &server_pere::open_socket_2, this);
	return check;
}

void server_pere::wait()
{
	pthread_join(thread_comm_client,NULL);
}
