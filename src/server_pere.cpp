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
#include "../xml/pugixml.hpp"




server_pere::~server_pere() {
	// TODO Auto-generated destructor stub
}


server_pere::server_pere(int sensorServerBox,int actuatorServerBox) :
		p_sensorServerBox(sensorServerBox), p_actuatorServerBox(actuatorServerBox)  {
	parse_home();
	//open_thread_comm_client();
}

int server_pere::parse_home()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("/Users/remi/Sites/GHome/GHOME/etc/home.xml");
    cout << "Load result: " << result.description() << endl;
    pugi::xml_node home = doc.child("home");
    cout << "Home : " << home << endl;
    pugi::xml_node rooms = home.child("rooms");
    cout << "Rooms : " << rooms << endl;
    for(pugi::xml_node_iterator room_it = rooms.begin(); room_it!=rooms.end(); ++room_it)
    {
    	pugi::xml_node room_name = room_it->child("name");
    	string s_name = room_name.value();
    	cout << s_name << endl;
    }
	return 0;
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
	communication_client comm_client(p_sensorServerBox, p_actuatorServerBox, p_fd);
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
	int sockfd;
	unsigned int size;
	struct sockaddr_in local;
	struct sockaddr_in remote;

	bzero(&local, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_port = htons(3006);
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
	p_fd = accept(sockfd, (struct sockaddr *)&remote, &size);
	cout << "File descriptor" << endl;
	cout << p_fd << endl ;
	int reussite;
	pthread_t thread_sock;
	reussite = pthread_create(&thread_sock, NULL, &server_pere::createCommClient_2, this);
	}

	close(sockfd);
	return(0);
}



int server_pere::open_thread_comm_client()
{

	pthread_t thread_comm_client;
	int check = pthread_create(&thread_comm_client, NULL, &server_pere::open_socket_2, this);
	return check;
}



