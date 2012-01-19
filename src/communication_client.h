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


class communication_client {
public:
	communication_client();
	communication_client(int sensorServerBox, int actuatorServerBox);
	virtual ~communication_client();
	void setSock(int sock);
private :
	int p_sock;

};

#endif /* COMMUNICATIONCLIENT_H_ */
