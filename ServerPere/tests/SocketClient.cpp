/*
 * SocketClient.cpp
 *
 *  Created on: 13 févr. 2012
 *      Author: remi
 */

#include "SocketClient.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/types.h>
#include <iostream>
using namespace std;
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

SocketClient::SocketClient() {
	// TODO Auto-generated constructor stub

}

SocketClient::~SocketClient() {
	// TODO Auto-generated destructor stub
}

void * SocketClient::sSocketClientCallBack(void* a_pPtr)
{
	SocketClient* p = (SocketClient*)a_pPtr;
	p->SocketClientT();
	return (0);
}

int SocketClient::SocketClientT()
{

	struct sockaddr_in sLocal;
	bzero(&sLocal, sizeof(sLocal));
	sLocal.sin_family = AF_INET;
	sLocal.sin_port = htons(ServerPere::SERVER_PERE_PORT);
	sLocal.sin_addr.s_addr = INADDR_ANY;
	bzero(&(sLocal.sin_zero), 8);
	m_iSocket = socket(AF_INET, SOCK_STREAM, 0);
	int on = 1;
	setsockopt(m_iSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	int iConnect = connect(m_iSocket, (struct sockaddr *)&sLocal,sizeof(sLocal));
	if(iConnect!=-1)
	{
		cout << "Connexion client"<<endl;
	} else {
		cout << "Probleme de connexion"<<endl;
	}

	return 0;
}

int SocketClient::InitializeSocket()
{
	pthread_t threadClient;
	pthread_create(&threadClient, NULL, &SocketClient::sSocketClientCallBack, this);
	return 0;
}

int SocketClient::SendMessage(char * buffer)
{
	int iMessSend=send(m_iSocket,buffer,sizeof(buffer),0);
	return iMessSend;
}

