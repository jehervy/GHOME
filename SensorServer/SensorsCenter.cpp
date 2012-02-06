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
#include "../Utils/GhomeBox.h"
#include "../xml/pugixml.hpp"
#include "../inference/Engine.h"
//#include "../monitoring/InferenceAction.h"


#define DROITS 0660 	// Droits d'accès
#define REFERENCE "." 	//Fichier courant utilisé pour bâtir la clé publique

SensorsCenter::SensorsCenter(int a_iBalServer, const char* a_pXmlFile) : m_iBalServer(a_iBalServer)
{
	ParserXML("src/etc/sensors.xml");
	m_iBalModel = msgget (ftok (REFERENCE, '2'), IPC_CREAT | DROITS );
	m_pModel = new EnOceanSensorModel(m_iBalModel);
	m_pModel->Start();
}

SensorsCenter::~SensorsCenter()
{
	m_pModel->Stop();
	msgctl(m_iBalModel,IPC_RMID,0);
}

void SensorsCenter::Start()
{
	pthread_create(&m_thread, NULL, sCallback, this);
}

void SensorsCenter::Stop()
{
	pthread_cancel(m_thread);
}

void SensorsCenter::Run()
{
	inference::Engine Engine("src/etc/rules.xml");

	int iSensorId = 0;
	int iSensorValue = 0;
	while(true)
	{
		if(GhomeBox::receive_message(m_iBalModel, iSensorId, iSensorValue))
		{
			mapSensors::iterator it = m_sensors.find(iSensorId);
			if(it != m_sensors.end())
			{
				GhomeBox::send_actuator_box(m_iBalServer, 1, it->second.first, it->second.second, iSensorValue);
				inference::Actions Actions = Engine.run(it->second.first, iSensorValue);
				for(unsigned int i=0; i<Actions.size(); i++)
				{
					//std::cout << "Nouvelle action " << i << std::endl;
					GhomeBox::send_actuator_box(m_iBalServer, 2, Actions[i].getMetric(), it->second.second, Actions[i].getValue() );
				}
			}
		}
	}
}

void SensorsCenter::ParserXML(const char* xmlFile)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(xmlFile);
	pugi::xml_node xmlSensors = doc.child("sensors");
	std::cout << "Load result: " << result.description() << std::endl;

	for (pugi::xml_node_iterator sensorsIt = xmlSensors.begin(); sensorsIt != xmlSensors.end(); ++sensorsIt)
	{
		std::pair<int, int> sensorNode;
		std::pair<int, std::pair<int, int> > mapNode;

		int iSensorId = sensorsIt->attribute("id").as_int();
		int iMetricId = atoi(sensorsIt->child("metric").child_value());
		int iRoomId = atoi(sensorsIt->child("room").child_value());

		sensorNode.first = iMetricId;
		sensorNode.second = iRoomId;

		mapNode.first = iSensorId;
		mapNode.second = sensorNode;

		this->m_sensors.insert(m_sensors.begin(), mapNode);

	}
}
