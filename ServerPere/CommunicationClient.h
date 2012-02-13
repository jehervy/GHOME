/*
 * CommunicationClient.h
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
#include "ServerPere.h"
#include "../Utils/GhomeBox.h"


class CommunicationClient {
public:
	CommunicationClient();
	/*
	 * Constructeur
	 */
	CommunicationClient(int a_iSensorServerBox, int a_iActuatorServerBox, int a_iFd, int a_iSock, void * a_pPtr);
	/*
	 * Constructeur
	 */
	virtual ~CommunicationClient();
	/*
	 * Destructeur
	 */
	void TransferMessage();
	/*
	 * Lis les messages envoyés sur le socket
	 */
	void FreeCreateBuffer(int a_iLongueur);
	/*
	 * Gere la creation et la liberation d'un buffer.
	 * Prend en parametre la taille du buffer cree.
	 */
	int ReadMessage(int a_iTailleALire);
	int ReadMessage(int a_TailleALire, int &a_iMessage, string a_sMessage);
	void EtatLecture();
private :
	int m_iSensorServerBox;
	int m_iActuatorServerBox;
	int m_iPFileDescriptor;
	bool m_bClientOpened;
	int m_iSocket;
	char * m_cBuffer;
	int m_iId = -1;
	int m_iMetric;
	int m_iRoom;
	int m_iValue;
	int iNbOctets;
	int iTailleALire;
	int iTailleMessage;
};

#endif /* COMMUNICATIONCLIENT_H_ */
