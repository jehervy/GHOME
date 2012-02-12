/*
 * DataContext.cpp
 *
 *  Created on: 22 janv. 2012
 *      Author: miroof
 */

#include "DataContext.h"
#include <string.h>
#include <sys/types.h>
#include <sys/msg.h>	//pour la boite aux lettres
# include <sys/ipc.h>
#include <string>
#include "../socket/ClientSocket.h"
#include "../socket/SocketException.h"
#include <pthread.h>

using namespace std;


void* DataContext::sRcvData(void* a_pArgs)
{
	// Récupération des informations de connection
	networkInfo infos = *((networkInfo*) a_pArgs);

	// Création de la structure de message pour la boîte aux lettres
	balMessage msg;
	msg.mtype = 1;

	try
	{
		// Création du socket client
		ClientSocket clientSocket ( infos.address, infos.port );
		string sTrame;
		try
		{
			// Récupération de la trame, formatage et envoie dans la file de messages
			while(true){
		  		clientSocket >> sTrame;
				bzero(msg.mtext, MSGSIZE);
		  		strcpy(msg.mtext, sTrame.c_str());
		  		msgsnd(infos.bal, &msg, MSGSIZE, 0);
			}
		}
	     catch ( SocketException& ) {
	    	 pthread_exit(NULL);
	     }

	    }
	  catch ( SocketException& e )
	    {
	      pthread_exit(NULL);
	    }

	return 0;
}


void* DataContext::sSndData(void* a_pArgs)
{
	// Récupération des informations de connection
	networkInfo infos = *((networkInfo*) a_pArgs);

	// Création de la structure de message pour la boîte aux lettres
	balMessage msg;

	try
	{
		// Création du socket client
		ClientSocket clientSocket ( infos.address, infos.port );
		try
		{
			// Récupération du message de la file de messages, envoi au serveur
			while(true){
				bzero(msg.mtext, MSGSIZE);
				if(msgrcv(infos.bal,&msg,MSGSIZE, 1, 0 ) != -1)
				{
					clientSocket << msg.mtext;
				}
			}
		}
		 catch ( SocketException& ) {
			 pthread_exit(NULL);
		 }

		}
	  catch ( SocketException& e )
		{
		  pthread_exit(NULL);
		}

	return 0;
}
