/*
 * SensorsCenter.cpp
 *
 *  Created on: 22 janv. 2012
 *      Author: miroof
 */

#include "SensorsCenter.h"
#include "../Utils/DataContext.h"
#include <iostream>
#include <algorithm>
#include <stdlib.h>
# include <sys/ipc.h>
#include <sys/msg.h>	//pour la boite aux lettres
#include "../Utils/GhomeBox.h"
#include "../xml/pugixml.hpp"
#include "../inference/Engine.h"
#include "../Utils/SystemLog.h"


#define DROITS 0660 	// Droits d'accès
#define REFERENCE "." 	//Fichier courant utilisé pour bâtir la clé publique

SensorsCenter::SensorsCenter(int a_iBalServer, const char* a_pXmlFile) : m_iBalServer(a_iBalServer)
{
	ParserXML(a_pXmlFile);
	m_iBalModel = msgget (IPC_PRIVATE, IPC_CREAT | DROITS );
	if(m_iBalModel == -1)
		SystemLog::AddLog(SystemLog::ERROR, "SensorsCenter : Creation BalModel");
	else
		SystemLog::AddLog(SystemLog::SUCCESS, "SensorCenter : Creation BalModel");
	m_pModel = new EnOceanSensorModel(m_iBalModel);
	m_pModel->Start();
}

SensorsCenter::~SensorsCenter()
{
	m_pModel->Stop();
	delete m_pModel;
	int ret = msgctl(m_iBalModel,IPC_RMID,0);
	if(ret != 0)
		SystemLog::AddLog(SystemLog::ERROR, "SensorsCenter : Suppression BalModel");
	else
		SystemLog::AddLog(SystemLog::SUCCESS, "SensorsCenter : Suppression BalModel");
}

void SensorsCenter::Start()
{
	int ret = pthread_create(&m_thread, NULL, sCallback, this);
	if(ret != 0)
		SystemLog::AddLog(SystemLog::ERROR, "SensorsCenter : Lancement du thread principal");
	else
		SystemLog::AddLog(SystemLog::SUCCESS, "SensorsCenter : Lancement du thread principal");
}

void SensorsCenter::Stop()
{
	int ret = pthread_cancel(m_thread);
	if(ret != 0)
		SystemLog::AddLog(SystemLog::ERROR, "SensorsCenter : Suppression du thread principal");
	else
		SystemLog::AddLog(SystemLog::SUCCESS, "SensorsCenter : Suppression du thread principal");
}

void SensorsCenter::Run()
{
	inference::Engine Engine("src/etc/rules.xml");

	int iSensorId = 0;
	int iSensorValue = 0;

	inference::Actions ActionsTmp;

	while(true)
	{
		if(GhomeBox::ReceiveMessage(m_iBalModel, iSensorId, iSensorValue))
		{

			mapSensors::iterator it = m_sensors.find(iSensorId);
			if(it != m_sensors.end())
			{

				GhomeBox::SendActuatorBox(m_iBalServer, 1, it->second.first, it->second.second, iSensorValue);
				inference::Actions Actions = Engine.run(it->second.first, iSensorValue);
				for(unsigned int i=0; i<Actions.size(); i++)
				{
					if(ActionsTmp.size() == 0 || std::find(ActionsTmp.begin(), ActionsTmp.end(), Actions[i]) == ActionsTmp.end())
					{
							GhomeBox::SendActuatorBox(m_iBalServer, 2, Actions[i].getMetric(), it->second.second, Actions[i].getValue() );
					}
				}
				ActionsTmp = Actions;
			}
		}
	}
}

void SensorsCenter::ParserXML(const char* xmlFile)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(xmlFile);
	pugi::xml_node xmlSensors = doc.child("sensors");

	if(result.status != pugi::status_ok)
		SystemLog::AddLog(SystemLog::ERROR, "SensorsCenter : Parsing fichier xml");
	else
		SystemLog::AddLog(SystemLog::SUCCESS, "SensorsCenter : Parsing fichier xml");

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
