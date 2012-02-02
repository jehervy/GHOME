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


vector<int> vect_fd;


server_pere::~server_pere()
/*
 * Constructeur
 */
{

	cout<<"papa est mort"<<endl;
}



server_pere::server_pere(int sensorServerBox,int actuatorServerBox) :
		p_sensorServerBox(sensorServerBox), p_actuatorServerBox(actuatorServerBox)
/*
 * Constructeur surcharge ;
 * Appel la méthode de création d'un socket dans un nouveau thread
 */
{
		open_thread_comm_client();
}




void *server_pere::createCommClient_2(void * ptr)
/*
 * Methode static ouverte dans un nouveau thread
 * et appelant la methode d'instanciation de client
 */
{
	server_pere* p = (server_pere*)ptr;
	p->createCommClient();
	return (0);
}

void *server_pere::createCommClient()
/*
 * Ouvre une instance de communication_client
 */
{
	cout << "Create comm client : Socket : " << p_fd << endl;
	communication_client comm_client(p_sensorServerBox, p_actuatorServerBox, p_fd, sockfd, this);
	return (0);
}

void *server_pere::callback(void * ptr)
/*
 * Methode static ouverte dans un nouveau thread
 * et appellant la méthode de gestion du socket
 */
{
	server_pere* p = (server_pere*)ptr;
	p->open_socket();
	return (0);
}

void *server_pere::open_socket()
/*
 * Ouvre un nouveau socket et attend la connexion
 * de nouveaux clients, cree un nouveau thread pour
 * chaque client
 */
{
	cout << "Open Socket" << endl;
	unsigned int size;
	struct sockaddr_in local;
	struct sockaddr_in remote;

	bzero(&local, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_port = htons(3023);
	local.sin_addr.s_addr = INADDR_ANY;
	bzero(&(local.sin_zero), 8);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	int on = 1;
	int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	ret = bind(sockfd, (struct sockaddr *)&local, sizeof(sockaddr));

	server_pere::set_opened(true);

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
			nb_connection=server_pere::insert_fd(p_fd);
			int reussite;
			pthread_t thread_sock;
			reussite = pthread_create(&thread_sock, NULL, &server_pere::createCommClient_2, this);
		}

	}

	return(0);
}



int server_pere::open_thread_comm_client()
/*
 * Cree un nouveau thread pour la gestion du socket
 */
{

	int check = pthread_create(&thread_comm_client, NULL, &server_pere::callback, this);
	return check;
}

void server_pere::wait()
/*
 * Bloque l'execution de la tache mere jusqu'à ce que le
 * thread de gestion de socket ne soit tue
 */
{
	pthread_join(thread_comm_client,NULL);
}


void server_pere::kill_thread()
/*
 * Ferme le socket et tue le thread de gestion
 * du socket
 */
{
	shutdown(sockfd, SHUT_RDWR);
	pthread_cancel(thread_comm_client);
}

int server_pere::insert_fd(int fd)
/*
 * Ajoute un nouveau file descriptor (client)
 * dans le vector.
 * Retourne le nombre de clients connectes.
 */
{
	vect_fd.push_back(fd);
	int size_vect=vect_fd.size();
	return size_vect;
}

int server_pere::delete_fd(int fd)
/*
 * Supprime un file descriptor (client)
 * du vector, et ferme la connexion avec ce
 * client.
 * Retourne le nombre de clients connectes.
 */
{
	int size_vect=vect_fd.size();
	bool element_found = false;
	int position = 0;
	int elem_pos;
	while((element_found==false) & (position<size_vect))
	{
		elem_pos=vect_fd.at(position);
		if(elem_pos==fd)
		{
			cout<<"Element trouve"<<endl;
			element_found=true;
			vect_fd.erase(vect_fd.begin()+position);
			close(fd);
		}else
		{
			position++;
		}

	}
	size_vect=vect_fd.size();
	return size_vect;
}

void server_pere::set_opened(bool etat)
/*
 * Met à jour l'etat du socket.
 */
{
	s_opened=etat;
}
