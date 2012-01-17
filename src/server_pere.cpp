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

void *createCommClient(void* ptr)
{
	//int sock;
	//sock = (int)*ptr;
	//communication_client comm_client();
	cout << "Create comm client : Socket" << endl;
	return (0);
}

void *open_socket(void * lire)
{
	cout << "Open Socket" << endl;
	int sockfd, newfd;
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
	newfd=accept(sockfd, (struct sockaddr *)&remote, &size);
	int reussite;
	pthread_t thread_sock;
	reussite = pthread_create(&thread_sock, NULL, createCommClient, (void*) newfd);

	}

	return(0);
}



void print_port(int p_port_ecoute)
{
	cout << "On Žcoute sur le port : ";
	cout <<p_port_ecoute << endl;
}

int open_thread_comm_client()
{

	char *ecrire = "test thread";
	pthread_t thread_comm_client;
	int check = pthread_create(&thread_comm_client, NULL, open_socket, (void*) ecrire);
	return check;
}

server_pere::~server_pere() {
	// TODO Auto-generated destructor stub
}


server_pere::server_pere(int port_ecoute) : p_port_ecoute(port_ecoute) {
	print_port(p_port_ecoute);

	open_thread_comm_client();

}

