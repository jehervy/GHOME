/*
 * SensorsCenter.cpp
 *
 *  Created on: 22 janv. 2012
 *      Author: miroof
 */

#include "SensorsCenter.h"
#include "DataContext.h"
#include <iostream>
#include <stdlib.h>
# include <sys/ipc.h>
#include <sys/msg.h>	//pour la boite aux lettres

#define DROITS 0660 	// Droits d'accès
#define REFERENCE "." 	//Fichier courant utilisé pour bâtir la clé publique

SensorsCenter::SensorsCenter(int balServer, int balMonitoring, const char* xmlFile) : balServer(balServer), balMonitoring(balMonitoring)
{
	//parserXML(xmlFile);
	balModel = msgget (ftok (REFERENCE, '1'), IPC_CREAT | DROITS );
	model = new EnOceanSensorModel(balModel);
	model->start();

	pthread_create(&thread, NULL, callback, this);
}

SensorsCenter::~SensorsCenter()
{
	model->stop();
	pthread_cancel(thread);
	msgctl(balModel,IPC_RMID,0);
}

void SensorsCenter::run()
{
	std::cout << "RUN SensorsCenter" << std::endl;
	balMessage msg;
	while(true)
	{
		if(msgrcv(balModel,&msg,MSGSIZE, 1, 0 ) != -1)
		{
			msgsnd(balServer, &msg, MSGSIZE, 0);
		}
	}
}

void SensorsCenter::parserXML(const char* xmlFile)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(xmlFile);
	pugi::xml_node xmlSensors = doc.child("sensors");
	std::cout << "Load result: " << result.description() << std::endl;

	for (pugi::xml_node_iterator sensorsIt = xmlSensors.begin(); sensorsIt != xmlSensors.end(); ++sensorsIt)
	{
		std::pair<int, int> sensorNode;

//		int ruleId = sensorsIt->attribute("id").as_int();
		int metricId = atoi(sensorsIt->child("metric").child_value());
		int roomId = atoi(sensorsIt->child("room").child_value());

		sensorNode.first = metricId;
		sensorNode.second = roomId;

		this->sensors.push_back(sensorNode);

	}
}
