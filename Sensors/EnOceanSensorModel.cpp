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

#ifdef TESTING
	#define XMLFILE "src/Sensors/tests/fileTest2.xml"
#else
	#define XMLFILE "src/etc/enOceanSensorsId.xml"
#endif



EnOceanSensorModel::EnOceanSensorModel(int a_iBal) : AbstractModel(a_iBal)
{
	m_iBalNetwork = msgget (IPC_PRIVATE, IPC_CREAT | DROITS );
	if(m_iBalNetwork == -1)
		SystemLog::AddLog(SystemLog::ERROR, "EnOceanSensorModel : Creation BalNetwork");
	else
		SystemLog::AddLog(SystemLog::SUCCESS, "EnOceanSensorModel : Creation BalNetwork");
	ParserXml(XMLFILE);
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
			sensorInfoNode.iMin = 0;
			sensorInfoNode.iMax = 0;

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
			if(string(msg.mtext).length() >= 24)
			{

				string sId = string(msg.mtext).substr(16,8);
				string sData = string(msg.mtext).substr(8,8);


				mapSensorInfo::iterator it = m_sensorInfo.find(sId);

				if(it != m_sensorInfo.end())
				{
					if((xstrtoi(sData.c_str()) & it->second.iValid) == it->second.iValid)
					{
						switch(it->second.iType)
						{
							case SensorInfo::NUMERIC:
							{
								string sSubData = sData.substr(it->second.iPosData, it->second.iLengthData);
								int iDataValue = abs(it->second.iMin - (xstrtoi(sSubData.c_str())*abs(it->second.iMax - it->second.iMin)/255));
								GhomeBox::SendMessage(m_iBalCenter, it->second.iVirtualId, iDataValue);
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

	int ret = pthread_create(&m_threadNetwork, NULL, DataContext::sRcvData, (void *)infos);
	if(ret != 0)
		SystemLog::AddLog(SystemLog::ERROR, "EnOceanSensorModel : Lancement du thread DataContext");
	else
		SystemLog::AddLog(SystemLog::SUCCESS, "EnOceanSensorModel : Lancement du thread DataContext");


	AbstractModel::Start();
}

void EnOceanSensorModel::Stop()
{
	int ret = pthread_cancel(m_threadNetwork);
	if(ret != 0)
		SystemLog::AddLog(SystemLog::ERROR, "EnOceanSensorModel : Suppression du thread DataContext");
	else
		SystemLog::AddLog(SystemLog::SUCCESS, "EnOceanSensorModel : Suppression du thread DataContext");
	AbstractModel::Stop();
	ret = msgctl(m_iBalNetwork,IPC_RMID,0);
	if(ret != 0)
		SystemLog::AddLog(SystemLog::ERROR, "EnOceanSensorModel : Suppression BalNetwork");
	else
		SystemLog::AddLog(SystemLog::SUCCESS, "EnOceanSensorModel : Suppression BalNetwork");
}
