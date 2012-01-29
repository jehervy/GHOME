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


communication_client::communication_client() {


}

communication_client::communication_client(int sensorServerBox, int actuatorServerBox, int fd, int sock) :
p_sensorServerBox(sensorServerBox), p_actuatorServerBox(actuatorServerBox), p_fd(fd), p_sock(sock){
transfer_message();

}

communication_client::~communication_client() {

}

void communication_client::free_create_buffer(int longueur)
{
	free(buffer);
	buffer = (char*) malloc(longueur);
	bzero(buffer,longueur);
}

void communication_client::transfer_message()
{
	cout << "On est dans la communication client" << endl;
	cout <<p_fd<<endl;
	   int n;

	   p_opened=true;
	   int taille_message;

	   /*Fonctionnement d'une lecture d'un ordre de pilotage
	    * d'un client :
	    * - lecture sur un octet : taille du message de metric
	    * - lecture du metric
	    * - lecture sur un octer : taille du message de room
	    * - lecture du room
	    * - lecture sur un octet : taille du message de value
	    */
	   int t_longueur = 1;
	   int m_longueur;
while(p_opened)
{
	communication_client::free_create_buffer(t_longueur);
	n = read(p_fd, buffer, t_longueur);
	if(n>0)
	{
		m_longueur = atoi(buffer);
		communication_client::free_create_buffer(m_longueur);
		n = read(p_fd, buffer, m_longueur);
		metric = atoi(buffer);
		if(metric!=0)
		{
			communication_client::free_create_buffer(t_longueur);
			n = read(p_fd, buffer, t_longueur);
			m_longueur = atoi(buffer);
			communication_client::free_create_buffer(m_longueur);
			n = read(p_fd, buffer, m_longueur);
			room = atoi(buffer);
			communication_client::free_create_buffer(t_longueur);
			n = read(p_fd, buffer, t_longueur);
			m_longueur = atoi(buffer);
			communication_client::free_create_buffer(m_longueur);
			n = read(p_fd, buffer, m_longueur);
			value = atoi(buffer);

			cout << "Metric : "<<metric<<endl;
			cout << "Room : "<<room<<endl;
			cout << "Value : "<<value<<endl;
		} else {
			//On souhaite fermer le socket
			p_opened=false;
			server_pere::s_opened=false;
			cout << "Ferme : "<<server_pere::s_opened <<endl;
		}

	}
		  /* if (n > 0)
		   {
			cout << "Nb octets : " << n << endl;
			string type_2(buffer);
			if(type_2=="0")
			{
				p_opened=false;
				server_pere::s_opened=false;
				cout << "Ferme : "<<server_pere::s_opened <<endl;
			}
				printf("Here is the message: %d\n", atoi(buffer));


		   }*/
		   n = write(p_fd,"I got your message",18);



}
cout<<"Sortie du premier while"<<endl;
close(p_fd);

}
