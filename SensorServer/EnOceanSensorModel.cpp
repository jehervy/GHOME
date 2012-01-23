/*
 * EnOceanSensorModel.cpp
 *
 *  Created on: 22 janv. 2012
 *      Author: miroof
 */

#include "EnOceanSensorModel.h"
#include "DataContext.h"
#include <iostream>
# include <sys/ipc.h>
#include <sys/msg.h>	//pour la boite aux lettres
#include <cstring>
#include <cstdlib>

#define DROITS 0660 	// Droits d'accès
#define REFERENCE "." 	//Fichier courant utilisé pour bâtir la clé publique

EnOceanSensorModel::EnOceanSensorModel(int bal) : AbstractModel(bal)
{
	balNetwork = msgget (ftok (REFERENCE, '1'), IPC_CREAT | DROITS );
}

void EnOceanSensorModel::run()
{
	std::cout << "RUN EnOceanSensorModel" << std::endl;
	balMessage msg;
	while(true)
	{
		if(msgrcv(balNetwork,&msg,MSGSIZE, 1, 0 ) != -1)
		{
			msgsnd(balCenter, &msg, MSGSIZE, 0);
		}
	}
}

void EnOceanSensorModel::start()
{
	AbstractModel::start();

	networkInfo *infos = (networkInfo *)malloc(sizeof(networkInfo));
	strcpy(infos->address, "localhost");
	infos->port = 30000;
	infos->bal = balNetwork;

	pthread_create(&thread, NULL, DataContext::rcvData, (void *)infos);
}

void EnOceanSensorModel::stop()
{
	pthread_cancel(threadNetwork);
	AbstractModel::stop();
	msgctl(balNetwork,IPC_RMID,0);
}
