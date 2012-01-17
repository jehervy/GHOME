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
	server_pere(int port_ecoute);
	virtual ~server_pere();
	void *createCommClient(void* ptr);
private :
	int p_port_ecoute;

};

#endif /* SERVERPERE_H_ */
