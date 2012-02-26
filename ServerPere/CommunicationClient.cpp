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

/*
 * Constructeur
 */
CommunicationClient::CommunicationClient()
{


}

/*
 * Constructeur surcharge.
 * Appelle la methode de gestion des messages lus sur le socket
 */
CommunicationClient::CommunicationClient(int a_iSensorServerBox, int a_iActuatorServerBox, int a_iFd, int a_iSock, void * a_pPtr) :
m_iSensorServerBox(a_iSensorServerBox), m_iActuatorServerBox(a_iActuatorServerBox), m_iPFileDescriptor(a_iFd), m_iSocket(a_iSock)
{
	papa = (ServerPere*)a_pPtr;
	CommunicationClient::TransferMessage();

}

/*
 * Destructeur
 */
CommunicationClient::~CommunicationClient()
{

}


/*
 * Declare un buffer ;
 * Lit le nombre d'octets passe en parametre sur le socket,
 * correspondant au nombre d'octets a lire pour le message suivant ;
 * Libere le buffer ;
 */
int CommunicationClient::ReadMessage(int a_iTailleALire)
{
	int iMessage;
	char * cBuffer = (char*)malloc(a_iTailleALire);
	iNbOctets=read(m_iPFileDescriptor, cBuffer, a_iTailleALire);
	if(iNbOctets>0)
	{
		iMessage = atoi(cBuffer);
	} else
	{
		iMessage=-1;
	}
	free(cBuffer);
	return iMessage;
}

/*
 * Declare un buffer ;
 * Lit un nombre d'octets passe en parametre sur le socket ;
 * Appelle la methode ReadMessage avec la valeur lue ;
 * Definit la valeur de la reference passee en parametre ;
 */
int CommunicationClient::ReadMessage(int a_iTailleALire, int &a_iMessage, string a_sMessage)
{
	int iTailleMessage;
	char * cBuffer = (char*)malloc(a_iTailleALire);
	iNbOctets=read(m_iPFileDescriptor, cBuffer, a_iTailleALire);
	if(iNbOctets>0)
		{
		iTailleMessage = atoi(cBuffer);
		a_iMessage=CommunicationClient::ReadMessage(iTailleMessage);
		if(a_iMessage!=-1)
			{
				SystemLog::AddLog(SystemLog::SUCCESS, "Lecture message client ("+a_sMessage+")");
			} else {
				SystemLog::AddLog(SystemLog::SUCCESS, "Lecture message client ("+a_sMessage);
			}
		} else
		{
			iNbOctets=-1;
		}
	free(cBuffer);
	return iNbOctets;
}


/*
 * Tant que le file descriptor est ouvert :
 * Lit un octet sur le socket et traite le message
 * en fonction de la valeur de cet octet ;
 * case 1 : ordre de lecture ; (non specifie)
 * case 2 : ordre de pilotage ;
 * case 3 : ordre de maintenance ; (non specifie)
 * case 4 : ordre de redémarrage du serveur ;
 */
void CommunicationClient::TransferMessage()

{
	   m_bClientOpened=true;
	   stringstream ss;
	   ss << m_iPFileDescriptor;
	   SystemLog::AddLog(SystemLog::SUCCESS, "Client "+ss.str()+" ouvert");

while(m_bClientOpened)
{
	m_iId = -1;
		m_iId=CommunicationClient::ReadMessage(1);
		if(m_iId==-1)
		{
			SystemLog::AddLog(SystemLog::ERROR, "Message client incorrect");
		}
	switch(m_iId){
			case 4 :
				m_bClientOpened=false;
				papa->SetOpened(false);
				papa->Stop();
				//TODO : redemarrer
				break;
			case 0 :
				m_bClientOpened=false;
				papa->DeleteFd(m_iPFileDescriptor);
				break;
			case 1 :
				break;
			case 2 :
				iNbOctets = CommunicationClient::ReadMessage(1, m_iMetric, "metric");
				iNbOctets = CommunicationClient::ReadMessage(1, m_iRoom, "room");
				iNbOctets = CommunicationClient::ReadMessage(1, m_iValue, "value");
			    //Ecriture du message dans la boite aux lettres actuator
				bool bSendMess;
				bSendMess=GhomeBox::SendActuatorBox(m_iActuatorServerBox, m_iId, m_iMetric, m_iRoom, m_iValue);
				if(bSendMess==true)
				{
					SystemLog::AddLog(SystemLog::SUCCESS, "Ordre de pilotage dans file message (client "+ss.str()+")");
				} else {
					SystemLog::AddLog(SystemLog::ERROR, "Ordre de pilotage dans file message, (client "+ss.str()+"), retour : ");
				}
				break;
			case 3 :
				break;

			}
}

}


