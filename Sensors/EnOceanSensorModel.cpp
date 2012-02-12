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

#include <cstdio>
#include<sys/errno.h>


#define DROITS 0660 	// Droits d'accès
#define REFERENCE "." 	//Fichier courant utilisé pour bâtir la clé publique



EnOceanSensorModel::EnOceanSensorModel(int a_iBal) : AbstractModel(a_iBal)
{
	m_iBalNetwork = msgget (ftok (REFERENCE, '3'), IPC_CREAT | DROITS );
	ParserXml("src/etc/enOceanSensorsId.xml");
}

EnOceanSensorModel::~EnOceanSensorModel()
{
	for(mapSensorInfo::iterator it = m_sensorInfo.begin(); it != m_sensorInfo.end(); it++)
	{
		free(it->first);
	}
}

void EnOceanSensorModel::ParserXml(const char *a_pXmlFile)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(a_pXmlFile);
	pugi::xml_node xmlSensors = doc.child("sensors");
	std::cout << "Load result: " << result.description() << std::endl;

	for (pugi::xml_node_iterator sensorsIt = xmlSensors.begin(); sensorsIt != xmlSensors.end(); ++sensorsIt)
	{
		SensorInfo sensorInfoNode;
		std::pair<char*, SensorInfo> mapNode;

		int virtualId = atoi(sensorsIt->child("virtualId").child_value());
		char *physicalId = (char*)malloc(sizeof(sensorsIt->child("physicalId").child_value()));
		std::strcpy(physicalId,sensorsIt->child("physicalId").child_value());

		sensorInfoNode.virtualId = virtualId;

		mapNode.first = physicalId;
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
			char *pId = GetId(msg.mtext);
			char *pData = GetData(msg.mtext);

			mapSensorInfo::iterator it = m_sensorInfo.find(pId);

			if(it != m_sensorInfo.end())
			{
				if(std::strcmp(pId, "00893386") == 0)
				{
					if((xstrtoi(pData) & 8) == 8)
					{
						char *substr = (char*)malloc(3*sizeof(char));
						strncpy(substr, pData+4, 2);
						substr[2] = '\0';

						GhomeBox::SendMessage(m_iBalCenter, it->second.virtualId, 40 - (xstrtoi(substr)*40/255));

						free(substr);
					}
				}
				else if(std::strcmp(pId, "0001B596") == 0)
				{
					if((xstrtoi(pData) & 8) == 8)
					{
						char *substr = (char*)malloc(3*sizeof(char));
						strncpy(substr, pData+6, 2);
						substr[2] = '\0';

						if (xstrtoi(substr)%2 == 0)
						{
							GhomeBox::SendMessage(m_iBalCenter, it->second.virtualId, 1);
						}
						else
						{
							GhomeBox::SendMessage(m_iBalCenter, it->second.virtualId, 0);
						}

						free(substr);
					}
				}
				else if(std::strcmp(pId, "0001B595") == 0)
				{
					if((xstrtoi(pData) & 8) == 8)
					{
						char *substr = (char*)malloc(3*sizeof(char));
						strncpy(substr, pData+6, 2);
						substr[2] = '\0';

						if (xstrtoi(substr)%2 == 0)
						{
							GhomeBox::SendMessage(m_iBalCenter, it->second.virtualId, 1);
						}
						else
						{
							GhomeBox::SendMessage(m_iBalCenter, it->second.virtualId, 0);
						}

						free(substr);
					}
				}
				else if(std::strcmp(pId, "0021CC07") == 0)
				{
					char *substr = (char*)malloc(3*sizeof(char));
					strncpy(substr, pData, 2);
					substr[2] = '\0';

					switch(xstrtoi(substr))
					{
						case 0:
						{
							GhomeBox::SendMessage(m_iBalCenter, it->second.virtualId, 0);
							break;
						}
						case 48:
						{
							GhomeBox::SendMessage(m_iBalCenter, it->second.virtualId, 1);
							break;
						}
						case 16:
						{
							GhomeBox::SendMessage(m_iBalCenter, it->second.virtualId, 2);
							break;
						}
						case 112:
						{
							GhomeBox::SendMessage(m_iBalCenter, it->second.virtualId, 3);
							break;
						}
						case 80:
						{
							GhomeBox::SendMessage(m_iBalCenter, it->second.virtualId, 4);
							break;
						}

						free(substr);
					}
				}
				else if(std::strcmp(pId, "00053E7b") == 0)
				{

				}
			}

			free(pId);
			free(pData);
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

char* EnOceanSensorModel::GetId(char *a_pData){
	char* res = (char*)malloc(9*sizeof(char));
	res[8] = '\0';
	strncpy(res,a_pData+16,8);
	return res;
}

char* EnOceanSensorModel::GetData(char *a_pData){
	char* res = (char*)malloc(9*sizeof(char));
	res[8] = '\0';
	strncpy(res,a_pData+8,8);
	return res;
}
