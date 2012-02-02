/*
 * communicationclient.cpp
 *
 *  Created on: 17 janv. 2012
 *      Author: remi
 */

#include "communication_client.h"
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

server_pere* papa;

communication_client::communication_client()
/*
 * Constructeur
 */
{


}

communication_client::communication_client(int sensorServerBox, int actuatorServerBox, int fd, int sock, void * ptr) :
p_sensorServerBox(sensorServerBox), p_actuatorServerBox(actuatorServerBox), p_fd(fd), p_sock(sock)
/*
 * Constructeur surcharge.
 * Appel la methode de gestion des messages lues sur le socket
 */
{
	papa = (server_pere*)ptr;
	transfer_message();

}

communication_client::~communication_client()
/*
 * Destructeur
 */
{

}

void communication_client::free_create_buffer(int longueur)
/*
 * Prends en parametre la longueur du buffer à initialisé,
 * après sa liberation memoire.
 */
{
	free(buffer);
	buffer = (char*) malloc(longueur);
	bzero(buffer,longueur);
}

void communication_client::transfer_message()
/*
 * Gere les messages recues sur le socket (envoyes par les clients).
 * Fonctionnement d'une lecture d'un ordre de pilotage
	    * d'un client :
	    * - lecture sur un octet : type du message (info, pilotage,...)
	    * 		- id = 0 : deconnexion du client
	    * 		- id = 4 : ordre de fermeture du server
	    * 		- id = 1 : info
	    * 		- id = 2 : pilotage
	    * 		- id = 3 : ordre de maintenance
	    * - lecture sur un octet : taille du message de metric
	    * - lecture du metric
	    * - lecture sur un octer : taille du message de room
	    * - lecture du room
	    * - lecture sur un octet : taille du message de value
 */
{

	   int n;
	   p_opened=true;
	   int t_longueur = 1;
	   int m_longueur;
while(p_opened)
{
	communication_client::free_create_buffer(t_longueur);
	n=read(p_fd, buffer, t_longueur);
	id=atoi(buffer);
	communication_client::free_create_buffer(t_longueur);
	cout<<"ID : "<<id<<endl;
	switch(id){
	case 4 :
		p_opened=false;
		papa->set_opened(false);
		papa->kill_thread();
		break;
	case 0 :
		p_opened=false;
		papa->delete_fd(p_fd);
		break;
	case 1 :
		break;
	case 2 :
		n = read(p_fd, buffer, t_longueur);
		m_longueur = atoi(buffer);
		communication_client::free_create_buffer(m_longueur);
		//Lecture de metric
		n = read(p_fd, buffer, m_longueur);
		metric = atoi(buffer);
		communication_client::free_create_buffer(t_longueur);
		//Nombre d'octets pour room
		n = read(p_fd, buffer, t_longueur);
		m_longueur = atoi(buffer);
		communication_client::free_create_buffer(m_longueur);
		//Lecture de room
		n = read(p_fd, buffer, m_longueur);
		room = atoi(buffer);
		communication_client::free_create_buffer(t_longueur);
		//Nombre d'octets pour value
		n = read(p_fd, buffer, t_longueur);
		m_longueur = atoi(buffer);
		communication_client::free_create_buffer(m_longueur);
		//Lecture de value
		n = read(p_fd, buffer, m_longueur);
		value = atoi(buffer);

		//Ecriture du message dans la boite aux lettres actuator
		ghome_box::send_actuator_box(p_actuatorServerBox, id, metric, room, value);

		break;
	case 3 :
		break;

	}




}

}
