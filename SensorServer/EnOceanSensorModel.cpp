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
	parserXml("/home/miroof/workspace/SensorServer/src/etc/enOceanSensorsId.xml");
}

void EnOceanSensorModel::parserXml(const char *xmlFile)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(xmlFile);
	pugi::xml_node xmlSensors = doc.child("sensors");
	std::cout << "Load result: " << result.description() << std::endl;

	for (pugi::xml_node_iterator sensorsIt = xmlSensors.begin(); sensorsIt != xmlSensors.end(); ++sensorsIt)
	{
		std::pair<int, const char*> sensorIdNode;

		int virtualId = atoi(sensorsIt->child("virtualId").child_value());
		const char *physicalId = sensorsIt->child("physicalId").child_value();

		sensorIdNode.first = virtualId;
		sensorIdNode.second = physicalId;

		this->sensorsId.push_back(sensorIdNode);

	}
}

void EnOceanSensorModel::run()
{
	std::cout << "RUN EnOceanSensorModel" << std::endl;
	balMessage msg;
	while(true)
	{
		if(msgrcv(balNetwork,&msg,MSGSIZE, 1, 0 ) != -1)
		{
//			char *id = get_id(msg.mtext);
//			char *data = get_id(msg.mtext);

			msgsnd(balCenter, &msg, MSGSIZE, 0);
		}
	}
}

void EnOceanSensorModel::start()
{
	AbstractModel::start();

	networkInfo *infos = (networkInfo *)malloc(sizeof(networkInfo));
	strcpy(infos->address, "localhost");
	infos->port = 5000;
	infos->bal = balNetwork;

	pthread_create(&thread, NULL, DataContext::rcvData, (void *)infos);
}

void EnOceanSensorModel::stop()
{
	pthread_cancel(threadNetwork);
	AbstractModel::stop();
	msgctl(balNetwork,IPC_RMID,0);
}

char* EnOceanSensorModel::get_id(char *data){
	char* res = (char*)malloc(9*sizeof(char));
	res[8] = '\0';
	strncpy(res,data+16,8);
	return res;
}

char* EnOceanSensorModel::get_data(char *data){
	char* res = (char*)malloc(9*sizeof(char));
	res[8] = '\0';
	strncpy(res,data+8,8);
	return res;
}
