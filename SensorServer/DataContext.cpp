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
#include <iostream>
#include <string>
#include "../socket/ClientSocket.h"
#include "../socket/SocketException.h"
#include <pthread.h>

using namespace std;


void* DataContext::rcvData(void* args)
{
	std::cout << "RUN rcvData" << std::endl;

	// Récupération des informations de connection
	networkInfo infos = *((networkInfo*) args);

	// Création de la structure de message pour la boîte aux lettres
	balMessage msg;
	msg.mtype = 1;

	try
	{
		// Création du socket client
		ClientSocket client_socket ( infos.address, infos.port );
		string trame;
		try
		{
			// Récupération de la trame, formatage et envoie dans la file de messages
			while(true){
		  		client_socket >> trame;
		  		strcpy(msg.mtext, trame.c_str());
		  		msgsnd(infos.bal, &msg, MSGSIZE, 0);
			}
		}
	     catch ( SocketException& ) {
	    	 std::cout << "Fail\n";
	    	 pthread_exit(NULL);
	     }

	    }
	  catch ( SocketException& e )
	    {
	      std::cout << "Exception was caught:" << e.description() << "\n";
	      pthread_exit(NULL);
	    }

	return 0;
}


void* DataContext::sndData(void* args)
{
	// Récupération des informations de connection
	networkInfo infos = *((networkInfo*) args);

	// Création de la structure de message pour la boîte aux lettres
	balMessage msg;

	try
	{
		// Création du socket client
		ClientSocket client_socket ( infos.address, infos.port );
		try
		{
			// Récupération du message de la file de messages, envoi au serveur
			while(true){
				if(msgrcv(infos.bal,&msg,MSGSIZE, 1, 0 ) != -1)
				{
					client_socket << msg.mtext;
				}
			}
		}
		 catch ( SocketException& ) {
			 std::cout << "Fail\n";
			 pthread_exit(NULL);
		 }

		}
	  catch ( SocketException& e )
		{
		  std::cout << "Exception was caught:" << e.description() << "\n";
		  pthread_exit(NULL);
		}

	return 0;
}
