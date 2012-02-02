/*
 * communicationclient.h
 *
 *  Created on: 17 janv. 2012
 *      Author: remi
 */

#ifndef COMMUNICATIONCLIENT_H_
#define COMMUNICATIONCLIENT_H_
#include <pthread.h>
#include <iostream>
using namespace std;
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "server_pere.h"
#include "../Utils/ghome_box.h"


class communication_client {
public:
	communication_client();
	/*
	 * Constructeur
	 */
	communication_client(int sensorServerBox, int actuatorServerBox, int fd, int sock, void * ptr);
	/*
	 * Constructeur
	 */
	virtual ~communication_client();
	/*
	 * Destructeur
	 */
	void transfer_message();
	/*
	 * Lis les messages envoyés sur le socket
	 */
	void free_create_buffer(int longueur);
	/*
	 * Gere la creation et la liberation d'un buffer.
	 * Prend en parametre la taille du buffer cree.
	 */
private :
	int p_sensorServerBox;
	int p_actuatorServerBox;
	int p_fd;
	bool p_opened;
	int p_sock;
	char * buffer;
	int id;
	int metric;
	int room;
	int value;

};

#endif /* COMMUNICATIONCLIENT_H_ */
