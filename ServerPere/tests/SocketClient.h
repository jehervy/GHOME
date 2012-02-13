/*
 * SocketClient.h
 *
 *  Created on: 13 févr. 2012
 *      Author: remi
 */

#ifndef SOCKETCLIENT_H_
#define SOCKETCLIENT_H_
#include "../ServerPere.h"

class SocketClient {
public:
	SocketClient();
	virtual ~SocketClient();
	int InitializeSocket();
	static void * sSocketClientCallBack(void* a_pPtr);
	int SocketClientT();
	int SendMessage(char * buffer);
private:
	int m_iSocket;
};

#endif /* SOCKETCLIENT_H_ */
