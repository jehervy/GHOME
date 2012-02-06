/*
 * serverpere.cpp
 *
 *  Created on: 16 janv. 2012
 *      Author: remi
 */

#include "ServerPere.h"
#include <iostream>
using namespace std;
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>


vector<int> m_vVectorFd;


ServerPere::~ServerPere()
/*
 * Constructeur
 */
{

}



ServerPere::ServerPere(int a_iSensorServerBox,int a_iActuatorServerBox) :
		m_iSensorServerBox(a_iSensorServerBox), m_iActuatorServerBox(a_iActuatorServerBox)
/*
 * Constructeur surcharge ;
 * Appel la méthode de création d'un socket dans un nouveau thread
 */
{
		ServerPere::OpenThreadCommClient();
}




void *ServerPere::sCreateCommClientCallBack(void * a_pPtr)
/*
 * Methode static ouverte dans un nouveau thread
 * et appelant la methode d'instanciation de client
 */
{
	ServerPere* p = (ServerPere*)a_pPtr;
	p->CreateCommClient();
	return (0);
}

void *ServerPere::CreateCommClient()
/*
 * Ouvre une instance de communication_client
 */
{
	cout << "Create comm client : Socket : " << m_iPFileDescriptor << endl;
	CommunicationClient comm_client(m_iSensorServerBox, m_iActuatorServerBox, m_iPFileDescriptor, m_iSockfd, this);
	return (0);
}

void *ServerPere::sOpenSocketCallBack(void * a_pPtr)
/*
 * Methode static ouverte dans un nouveau thread
 * et appellant la méthode de gestion du socket
 */
{
	ServerPere* p = (ServerPere*)a_pPtr;
	p->OpenSocket();
	return (0);
}

void *ServerPere::OpenSocket()
/*
 * Ouvre un nouveau socket et attend la connexion
 * de nouveaux clients, cree un nouveau thread pour
 * chaque client
 */
{
	cout << "Open Socket" << endl;
	unsigned int iSize;
	struct sockaddr_in sLocal;
	struct sockaddr_in sRemote;

	bzero(&sLocal, sizeof(sLocal));
	sLocal.sin_family = AF_INET;
	sLocal.sin_port = htons(3023);
	sLocal.sin_addr.s_addr = INADDR_ANY;
	bzero(&(sLocal.sin_zero), 8);
	m_iSockfd = socket(AF_INET, SOCK_STREAM, 0);
	int on = 1;
	int iRet = setsockopt(m_iSockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	iRet = bind(m_iSockfd, (struct sockaddr *)&sLocal, sizeof(sockaddr));
	if(iRet!=0)
	{
		//TODO : errno
		ServerPere::SetOpened(false);
	} else
	{
		cout<<"Bind reussi"<<endl;
		ServerPere::SetOpened(true);
		if(listen(m_iSockfd, 5) == -1)
		{
			perror("listen");
			exit(1);
		}
		iSize = sizeof(struct sockaddr_in);
		while(m_bSocketOpened)
			{

				m_iPFileDescriptor = accept(m_iSockfd, (struct sockaddr *)&sRemote, &iSize);
				if((m_iPFileDescriptor>0)&(m_bSocketOpened))
				{
					m_iNbConnection=ServerPere::InsertFd(m_iPFileDescriptor);
					//int reussite;
					pthread_t thread_sock;
					/*reussite = */pthread_create(&thread_sock, NULL, &ServerPere::sCreateCommClientCallBack, this);
				}

			}
	}

	return(0);
}



int ServerPere::OpenThreadCommClient()
/*
 * Cree un nouveau thread pour la gestion du socket
 */
{

	int iCheck = pthread_create(&m_ptThreadCommClient, NULL, &ServerPere::sOpenSocketCallBack, this);
	return iCheck;
}

void ServerPere::Wait()
/*
 * Bloque l'execution de la tache mere jusqu'à ce que le
 * thread de gestion de socket ne soit tue
 */
{
	pthread_join(m_ptThreadCommClient,NULL);
}


void ServerPere::KillThread()
/*
 * Ferme le socket et tue le thread de gestion
 * du socket
 */
{
	shutdown(m_iSockfd, SHUT_RDWR);
	pthread_cancel(m_ptThreadCommClient);
}

int ServerPere::InsertFd(int a_iFd)
/*
 * Ajoute un nouveau file descriptor (client)
 * dans le vector.
 * iRetourne le nombre de clients connectes.
 */
{
	m_vVectorFd.push_back(a_iFd);
	int iSizeVect=m_vVectorFd.size();
	return iSizeVect;
}

int ServerPere::DeleteFd(int a_iFd)
/*
 * Supprime un file descriptor (client)
 * du vector, et ferme la connexion avec ce
 * client.
 * iRetourne le nombre de clients connectes.
 */
{
	int iSize_vect=m_vVectorFd.size();
	bool element_found = false;
	int position = 0;
	int elem_pos;
	while((element_found==false) & (position<iSize_vect))
	{
		elem_pos=m_vVectorFd.at(position);
		if(elem_pos==a_iFd)
		{
			element_found=true;
			m_vVectorFd.erase(m_vVectorFd.begin()+position);
			close(a_iFd);
		}else
		{
			position++;
		}

	}
	iSize_vect=m_vVectorFd.size();
	return iSize_vect;
}

void ServerPere::SetOpened(bool a_bEtat)
/*
 * Met à jour l'etat du socket.
 */
{
	m_bSocketOpened=a_bEtat;
}
