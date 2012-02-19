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

/*
 * Destructeur
 */
ServerPere::~ServerPere()
{

}

/*
 * Constructeur
 */
ServerPere::ServerPere()
{

}

/*
 * Constructeur surcharge ;
 * Appel la mŽthode de crŽation d'un socket dans un nouveau thread
 */
ServerPere::ServerPere(int a_iSensorServerBox,int a_iActuatorServerBox) :
		m_iSensorServerBox(a_iSensorServerBox), m_iActuatorServerBox(a_iActuatorServerBox)
{

}

/*
 * Methode static ouverte dans un nouveau thread
 * et appelant la methode d'instanciation de client
 */
void *ServerPere::sCreateCommClientCallBack(void * a_pPtr)
{
	ServerPere* p = (ServerPere*)a_pPtr;
	p->CreateCommClient();
	return (0);
}

/*
 * Ouvre une instance de communication_client
 */
void *ServerPere::CreateCommClient()
{
	cout << "Create comm client : Socket : " << m_iPFileDescriptor << endl;
	CommunicationClient comm_client(m_iSensorServerBox, m_iActuatorServerBox, m_iPFileDescriptor, m_iSockfd, this);
	return (0);
}

/*
 * Methode static ouverte dans un nouveau thread
 * et appellant la methode de gestion du socket
 */
void *ServerPere::sOpenSocketCallBack(void * a_pPtr)
{
	cout<<"Callback"<<endl;
	ServerPere* p = (ServerPere*)a_pPtr;
	p->OpenSocket();
	return (0);
}

/*
 * Ouvre un nouveau socket et attend la connexion
 * de nouveaux clients, cree un nouveau thread pour
 * chaque client
 */
void *ServerPere::OpenSocket()
{
	cout<<"open"<<endl;
	unsigned int iSize;
	struct sockaddr_in sLocal;
	struct sockaddr_in sRemote;

	bzero(&sLocal, sizeof(sLocal));
	sLocal.sin_family = AF_INET;
	sLocal.sin_port = htons(ServerPere::SERVER_PERE_PORT);
	sLocal.sin_addr.s_addr = INADDR_ANY;
	bzero(&(sLocal.sin_zero), 8);
	m_iSockfd = socket(AF_INET, SOCK_STREAM, 0);
	int on = 1;
	int iRet = setsockopt(m_iSockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	iRet = bind(m_iSockfd, (struct sockaddr *)&sLocal, sizeof(sockaddr));
	if(iRet!=0)
	{
		SystemLog::AddLog(SystemLog::ERROR, "Bind socket serveur, retour : "+errno);
		ServerPere::SetOpened(false);
	} else
	{
		SystemLog::AddLog(SystemLog::SUCCESS, "Bind socket serveur");
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
				SystemLog::AddLog(SystemLog::SUCCESS, "Serveur en attente de connexion client");

				if((m_iPFileDescriptor>0)&(m_bSocketOpened))
				{
					m_iNbConnection=ServerPere::InsertFd(m_iPFileDescriptor);
					SystemLog::AddLog(SystemLog::SUCCESS, "Connexion d'un client");
					int iReussite;
					pthread_t thread_sock;
					iReussite = pthread_create(&thread_sock, NULL, &ServerPere::sCreateCommClientCallBack, this);
					if(iReussite==0)
					{
						SystemLog::AddLog(SystemLog::SUCCESS, "Thread de client ouvert");
					} else {
						SystemLog::AddLog(SystemLog::ERROR, "Thread de client non ouvert, retour : "+errno);
					}
				}

			}
	}

	return(0);
}


/*
 * Cree un nouveau thread pour la gestion du socket
 */
int ServerPere::Start()
{
	cout<<"start"<<endl;
	int iCheck = pthread_create(&m_ptThreadCommClient, NULL, &ServerPere::sOpenSocketCallBack, this);
	return iCheck;
}

/*
 * Bloque l'execution de la tache mere jusqu'ˆ ce que le
 * thread de gestion de socket ne soit tue
 */
void ServerPere::Wait()
{
	pthread_join(m_ptThreadCommClient,NULL);
}


/*
 * Ferme le socket et tue le thread de gestion
 * du socket
 */
void ServerPere::Stop()
{
	shutdown(m_iSockfd, SHUT_RDWR);
	pthread_cancel(m_ptThreadCommClient);
}

/*
 * Ajoute un nouveau file descriptor (client)
 * dans le vector.
 * iRetourne le nombre de clients connectes.
 */
int ServerPere::InsertFd(int a_iFd)
{
	m_vVectorFd.push_back(a_iFd);
	int iSizeVect=m_vVectorFd.size();
	return iSizeVect;
}

/*
 * Retourne un entier File Descriptor, en
 * parcourant le vector
 */
int ServerPere::GetFd(int a_iPosition)
{
	if((a_iPosition<m_vVectorFd.size()) & (a_iPosition>=0))
	{
		int iFd=m_vVectorFd.at(a_iPosition);
		return iFd;
	} else
	{
		return -1;
	}
}

/*
 * Supprime un file descriptor (client)
 * du vector, et ferme la connexion avec ce
 * client.
 * iRetourne le nombre de clients connectes.
 */
int ServerPere::DeleteFd(int a_iFd)
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

/*
 * Supprime un file descriptor (client)
 * du vector, et ferme la connexion avec ce
 * client.
 * iRetourne le nombre de clients connectes.
 */
void ServerPere::SetOpened(bool a_bEtat)
{
	m_bSocketOpened=a_bEtat;
}
