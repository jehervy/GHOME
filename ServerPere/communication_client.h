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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "server_pere.h"
using namespace std;


class communication_client {
public:
	communication_client();
	communication_client(int sensorServerBox, int actuatorServerBox, int fd, int sock);
	virtual ~communication_client();
	void transfer_message();
	void free_create_buffer(int longueur);
private :
	int p_sensorServerBox;
	int p_actuatorServerBox;
	int p_fd;
	bool p_opened;
	int p_sock;
	char * buffer;
	int metric;
	int room;
	int value;
};

#endif /* COMMUNICATIONCLIENT_H_ */
