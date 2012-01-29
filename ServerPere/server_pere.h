/*
 * serverpere.h
 *
 *  Created on: 16 janv. 2012
 *      Author: remi
 */

#ifndef SERVERPERE_H_
#define SERVERPERE_H_
#include <pthread.h>
#include "communication_client.h"


class server_pere {
public:
	server_pere();
	server_pere(int sensorServerBox, int actuatorServerBox);
	virtual ~server_pere();
	int open_thread_comm_client();
	void *createCommClient();
	static void  *createCommClient_2(void* ptr);
	void *open_socket();
	static void *open_socket_2(void* ptr);
    static void close_all();
	static 	bool  s_opened;
	void wait();
private :
	int p_sensorServerBox;
	int p_actuatorServerBox;
	int p_fd;
	int sockfd;
	pthread_t thread_comm_client;
};

#endif /* SERVERPERE_H_ */
