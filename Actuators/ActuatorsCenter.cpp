/*
 * SensorsCenter.cpp
 *
 *  Created on: 22 janv. 2012
 *      Author: Xav
 */

#include "ActuatorsCenter.h"
#include "../Utils/GhomeBox.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>	//pour la boite aux lettres

using namespace std;

#define DROITS 0660 	// Droits d'acces
#define REFERENCE "." 	//Fichier courant utilise pour batir la cle publique

ActuatorsCenter::ActuatorsCenter(int a_iBalServer, const string a_sXmlFile) : m_iBalServer(a_iBalServer)
{
	parserXML(a_sXmlFile);
	m_iBalModel = msgget (ftok (REFERENCE, '2'), IPC_CREAT | DROITS );
	//m_model = new EnOceanActuatorModel(m_iBalModel);
	m_model->start();

	pthread_create(&m_thread, NULL, callback, this);
}

ActuatorsCenter::~ActuatorsCenter()
{
	m_model->stop();
	pthread_cancel(m_thread);
	msgctl(m_iBalModel,IPC_RMID,0);
}

void ActuatorsCenter::run()
{
	cout << "RUN ActuatorsCenter" << endl;
	while(true)
	{
		int iTypeMes, iMetric, iRoom, iValue=0;
		GhomeBox::ReceiveMessage(m_iBalServer, iTypeMes, iMetric, iRoom, iValue);

		cout<<"j'ai recu un message : " << iTypeMes << " " << iMetric << " " << iRoom << " " << iValue << endl;

		if (iTypeMes == 2) // Correspond � un ordre de pilotage
		{
			cout << "ordre de pilotage en traitement" << endl;
			int iVirtualId = findVirtualId(iMetric, iRoom);
			if (iVirtualId != -1)
			{
				cout << "Id virtuel de l'actionneur : "<< iVirtualId << endl;
				GhomeBox::SendMessage(m_iBalModel,iVirtualId, iValue);
				cout << "message envoy� � la bal Model " << m_iBalModel<< endl;
			}
			else
				cout << "Aucun capteur ne correspond � ces informations :" << endl << "metric : " << iMetric << ", room : " << iRoom << "." << endl;
		}
		else
			cout << "Warning : le message re�ue par ActuatorCenter n'est pas un ordre de pilotage. Ce dernier sera ignor�." << endl;
	}
}


void ActuatorsCenter::parserXML(const string a_sXmlFile)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(a_sXmlFile.c_str());
	pugi::xml_node xmlActuators = doc.child("actuators");
	cout << "Load result: " << result.description() << endl;

	for (pugi::xml_node_iterator actuatorsIt = xmlActuators.begin(); actuatorsIt != xmlActuators.end(); ++actuatorsIt)
	{
		std::pair<std::pair<int,int>,int>  actuatorNode;

		int iVirtualId = actuatorsIt->attribute("id").as_int();
		int iMetricId = atoi(actuatorsIt->child("metric").child_value());
		int iRoomId = atoi(actuatorsIt->child("room").child_value());

		(actuatorNode.first).first = iMetricId;
		(actuatorNode.first).second = iRoomId;
		actuatorNode.second = iVirtualId;

		this->m_actuators.insert(actuatorNode);
	}
}

int ActuatorsCenter::findVirtualId(int a_iMetric, int a_iRoom)
{
	mapActuators::iterator itMapActuators;
	itMapActuators=this->m_actuators.find(std::pair<int,int> (a_iMetric, a_iRoom));
	int iRes = (itMapActuators != this->m_actuators.end())? itMapActuators->second : -1;
	return iRes;
}
