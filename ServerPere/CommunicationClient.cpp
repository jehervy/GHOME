/*
 * communicationclient.cpp
 *
 *  Created on: 17 janv. 2012
 *      Author: remi
 */

#include "CommunicationClient.h"
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

ServerPere* papa;

CommunicationClient::CommunicationClient()
/*
 * Constructeur
 */
{


}

CommunicationClient::CommunicationClient(int a_iSensorServerBox, int a_iActuatorServerBox, int a_iFd, int a_iSock, void * a_pPtr) :
m_iSensorServerBox(a_iSensorServerBox), m_iActuatorServerBox(a_iActuatorServerBox), m_iPFileDescriptor(a_iFd), m_iSocket(a_iSock)
/*
 * Constructeur surcharge.
 * Appel la methode de gestion des messages lues sur le socket
 */
{
	papa = (ServerPere*)a_pPtr;
	CommunicationClient::TransferMessage();

}

CommunicationClient::~CommunicationClient()
/*
 * Destructeur
 */
{

}

void CommunicationClient::FreeCreateBuffer(int a_iLongueur)
/*
 * Prends en parametre la longueur du m_cBuffer à initialisé,
 * après sa liberation memoire.
 */
{
	free(m_cBuffer);
	m_cBuffer = (char*) malloc(a_iLongueur);
	bzero(m_cBuffer,a_iLongueur);
}

int CommunicationClient::ReadMessage(int a_iTailleALire)
{
	int iMessage;
	CommunicationClient::FreeCreateBuffer(a_iTailleALire);
	iNbOctets=read(m_iPFileDescriptor, m_cBuffer, a_iTailleALire);
	cout << "Octets : " << iNbOctets << endl;
	if(iNbOctets>0)
	{
		iMessage = atoi(m_cBuffer);
	}

	return iMessage;
}

int CommunicationClient::ReadMessage(int a_iTailleALire, int &a_iMessage)
{
	int iTailleMessage;
	CommunicationClient::FreeCreateBuffer(a_iTailleALire);
	iNbOctets=read(m_iPFileDescriptor, m_cBuffer, a_iTailleALire);
	if(iNbOctets>0)
		{
		iTailleMessage = atoi(m_cBuffer);
		a_iMessage=CommunicationClient::ReadMessage(iTailleMessage);
		}
	return iNbOctets;
}


void CommunicationClient::TransferMessage()

{
	   m_bClientOpened=true;

while(m_bClientOpened)
{
	m_iId=CommunicationClient::ReadMessage(1);//On lit le type de message

		switch(m_iId){
			case 4 :
				m_bClientOpened=false;
				papa->SetOpened(false);
				papa->Stop();
				break;
			case 0 :
				m_bClientOpened=false;
				papa->DeleteFd(m_iPFileDescriptor);
				break;
			case 1 :
				break;
			case 2 :
				iNbOctets = CommunicationClient::ReadMessage(1, m_iMetric);
				cout<<"Nb octets : "<< iNbOctets << endl;
				iNbOctets = CommunicationClient::ReadMessage(1, m_iRoom);
				cout << "Nb octets : " << iNbOctets << endl;
				iNbOctets = CommunicationClient::ReadMessage(1, m_iValue);
				cout << "Nb octets : " << iNbOctets << endl;
				cout << "Metric : " << m_iMetric << endl;
				cout << "Room : " << m_iRoom << endl;
				cout << "Value : " << m_iValue << endl;
			//Ecriture du message dans la boite aux lettres actuator
			GhomeBox::SendActuatorBox(m_iActuatorServerBox, m_iId, m_iMetric, m_iRoom, m_iValue);

			break;
		case 3 :
			break;

		}
	}

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

}


