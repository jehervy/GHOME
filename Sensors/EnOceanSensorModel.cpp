/*
 * EnOceanSensorModel.cpp
 *
 *  Created on: 22 janv. 2012
 *      Author: miroof
 */

#include "EnOceanSensorModel.h"
#include "../Utils/DataContext.h"
#include <iostream>
# include <sys/ipc.h>
#include <sys/msg.h>	//pour la boite aux lettres
#include <cstdlib>
#include "../Utils/GhomeBox.h"
#include "../Utils/utils.h"
#include "../xml/pugixml.hpp"
#include "../Utils/SystemLog.h"
#include "../gmem/gmem.h"
#include <cstdio>
#include <sys/errno.h>
#include <string.h>

using namespace std;

#define DROITS 0660 	// Droits d'accès
#define REFERENCE "." 	//Fichier courant utilisé pour bâtir la clé publique



EnOceanSensorModel::EnOceanSensorModel(int a_iBal) : AbstractModel(a_iBal)
{
	m_iBalNetwork = msgget (IPC_PRIVATE, IPC_CREAT | DROITS );
	if(m_iBalNetwork == -1)
		SystemLog::AddLog(SystemLog::ERROR, "EnOceanSensorModel : Création BalNetwork");
	else
		SystemLog::AddLog(SystemLog::SUCCESS, "EnOceanSensorModel : Création BalNetwork");
	ParserXml("src/etc/enOceanSensorsId.xml");
}

EnOceanSensorModel::~EnOceanSensorModel()
{

}

void EnOceanSensorModel::ParserXml(const char *a_pXmlFile)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(a_pXmlFile);
	pugi::xml_node xmlSensors = doc.child("sensors");
	if(result.status != pugi::status_ok)
		SystemLog::AddLog(SystemLog::ERROR, "EnOceanSensorModel : Parsing fichier xml");
	else
		SystemLog::AddLog(SystemLog::SUCCESS, "EnOceanSensorModel : Parsing fichier xml");
	std::cout << "Load result: " << result.description() << std::endl;

	for (pugi::xml_node_iterator sensorsIt = xmlSensors.begin(); sensorsIt != xmlSensors.end(); ++sensorsIt)
	{
		SensorInfo sensorInfoNode;
		std::pair<string, SensorInfo> mapNode;

		int virtualId = atoi(sensorsIt->child("virtualId").child_value());
		string sPhysicalId = string(sensorsIt->child("physicalId").child_value());

		sensorInfoNode.iVirtualId = virtualId;
		sensorInfoNode.iValid = atoi(sensorsIt->child("valid").child_value());
		pugi::xml_node dataNode = sensorsIt->child("data");
		string sType = dataNode.attribute("type").value();
		sensorInfoNode.iPosData = dataNode.attribute("pos").as_int();
		sensorInfoNode.iLengthData = dataNode.attribute("length").as_int();
		if(sType == "numeric")
		{
			sensorInfoNode.iType = SensorInfo::NUMERIC;
			sensorInfoNode.iMin = atoi(string(dataNode.child("min").child_value()).c_str());
			sensorInfoNode.iMax = atoi(string(dataNode.child("max").child_value()).c_str());
		}
		else if(sType == "binary")
		{
			sensorInfoNode.iType = SensorInfo::BINARY;

			for (pugi::xml_node_iterator valueIt = dataNode.begin(); valueIt != dataNode.end(); ++valueIt)
			{
				std::pair<int, int> mapValueNode;

				mapValueNode.first = valueIt->attribute("data").as_int();
				mapValueNode.second = atoi(valueIt->child_value());

				sensorInfoNode.mapValue.insert(sensorInfoNode.mapValue.begin(), mapValueNode);

			}

		}

		mapNode.first = sPhysicalId;
		mapNode.second = sensorInfoNode;

		this->m_sensorInfo.insert(this->m_sensorInfo.begin(), mapNode);
	}
}

void EnOceanSensorModel::Run()
{
	balMessage msg;
	while(true)
	{
		bzero(msg.mtext, MSGSIZE);
		if(msgrcv(m_iBalNetwork,&msg,MSGSIZE, 1, 0) != -1)
		{

			string sId = string(msg.mtext).substr(16,8);
			string sData = string(msg.mtext).substr(8,8);

			mapSensorInfo::iterator it = m_sensorInfo.find(sId);

			if(it != m_sensorInfo.end())
			{
				std::cout << "capteur connu " << std::endl;
				if((xstrtoi(sData.c_str()) & it->second.iValid) == it->second.iValid)
				{
					std::cout << "capteur valide " << std::endl;
					switch(it->second.iType)
					{
						case SensorInfo::NUMERIC:
						{
							string sSubData = sData.substr(it->second.iPosData, it->second.iLengthData);
							GhomeBox::SendMessage(m_iBalCenter, it->second.iVirtualId, it->second.iMax - (xstrtoi(sSubData.c_str())*it->second.iMax/255));
							break;
						}
						case SensorInfo::BINARY:
						{
							string sSubData = sData.substr(it->second.iPosData, it->second.iLengthData);
							map<int, int>::iterator valueIt = it->second.mapValue.find(xstrtoi(sSubData.c_str()));
							if(valueIt != it->second.mapValue.end())
							{
								GhomeBox::SendMessage(m_iBalCenter, it->second.iVirtualId, valueIt->second);
							}
							break;
						}
					}
				}
			}
		}
		else
		{
			perror( strerror(errno) );
			printf("msgrcv failed");
		}
	}
}

void EnOceanSensorModel::Start()
{
	networkInfo *infos = (networkInfo *)malloc(sizeof(networkInfo));
	strcpy(infos->address, "localhost");
	infos->port = 5000;
	infos->bal = m_iBalNetwork;

	pthread_create(&m_thread, NULL, DataContext::sRcvData, (void *)infos);

	AbstractModel::Start();
}

void EnOceanSensorModel::Stop()
{
	pthread_cancel(m_threadNetwork);
	AbstractModel::Stop();
	msgctl(m_iBalNetwork,IPC_RMID,0);
}
