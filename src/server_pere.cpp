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

server_pere::~server_pere() {
	// TODO Auto-generated destructor stub
}


server_pere::server_pere(int sensorServerBox,int actuatorServerBox) : p_sensorServerBox(sensorServerBox), p_actuatorServerBox(actuatorServerBox)  {

	open_thread_comm_client();
}


void *server_pere::createCommClient(void* ptr)
{
	int sock;
	int*  data = reinterpret_cast<int*>(ptr);
	sock = *data;
	delete data;
	communication_client comm_client(p_sensorServerBox, p_actuatorServerBox);
	cout << "Create comm client : Socket" << endl;
	return (0);
}

void *open_socket(void * lire)
{
	cout << "Open Socket" << endl;
	int sockfd;
	int *newfd;
	unsigned int size;
	struct sockaddr_in local;
	struct sockaddr_in remote;

	bzero(&local, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_port = htons(2301);
	local.sin_addr.s_addr = INADDR_ANY;
	bzero(&(local.sin_zero), 8);

	if((sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
	perror("socket");
	exit(1);
	}
	if(bind(sockfd, (struct sockaddr *)&local, sizeof(struct sockaddr)) == -1)
	{
	perror("bind");
	exit(1);
	}
	if(listen(sockfd, 5) == -1)
	{
	perror("listen");
	exit(1);
	}
	size = sizeof(struct sockaddr_in);

	while(1)
	{
	*newfd = accept(sockfd, (struct sockaddr *)&remote, &size);
	int reussite;
	pthread_t thread_sock;
	reussite = pthread_create(&thread_sock, NULL, createCommClient, (void*) newfd);

	}
	close(sockfd);
	return(0);
}



int server_pere::open_thread_comm_client()
{
	int *ecrire;
	pthread_t thread_comm_client;
	int check = pthread_create(&thread_comm_client, NULL, open_socket, (void*) ecrire);
	return check;
}



