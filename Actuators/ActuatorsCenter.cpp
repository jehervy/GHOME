/*
 * SensorsCenter.cpp
 *
 *  Created on: 22 janv. 2012
 *      Author: Xav
 */

#include "ActuatorsCenter.h"
#include "EnOceanActuatorModel.h"
#include "../Utils/DataContext.h"
#include "../Utils/GhomeBox.h"
#include "../Utils/SystemLog.h"
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
	if(m_iBalModel == -1)
		SystemLog::AddLog(SystemLog::ERROR, "ActuatorCenter : Reception message actuatorServerBox");
	else SystemLog::AddLog(SystemLog::SUCCESS, "ActuatorCenter : Reception message actuatorServerBox");

	m_pModel = new EnOceanActuatorModel(m_iBalModel);
	SystemLog::AddLog(SystemLog::SUCCESS, "ActuatorCenter : Creation du modele");

	m_pModel->Start();
}

ActuatorsCenter::~ActuatorsCenter()
{
	m_pModel->Stop();
	msgctl(m_iBalModel,IPC_RMID,0);
}

void ActuatorsCenter::Start()
{
	if (pthread_create(&m_pThread, NULL, callback, this)==0)
		SystemLog::AddLog(SystemLog::SUCCESS, "ActuatorCenter : Lancement du thread");
	else
		SystemLog::AddLog(SystemLog::ERROR, "ActuatorCenter : Lancement du thread");
}

void ActuatorsCenter::Stop()
{
	if (pthread_cancel(m_pThread)==0)
		SystemLog::AddLog(SystemLog::SUCCESS, "ActuatorCenter : Annulation du thread");
	else
		SystemLog::AddLog(SystemLog::ERROR, "ActuatorCenter : Annulation du thread");
}


void ActuatorsCenter::Run()
{
	while(true)
	{
		int iTypeMes, iMetric, iRoom, iValue=0;

		if (GhomeBox::ReceiveMessage(m_iBalServer, iTypeMes, iMetric, iRoom, iValue))
			SystemLog::AddLog(SystemLog::SUCCESS, "ActuatorCenter : message receptionne de actuatorServerBox");
		else SystemLog::AddLog(SystemLog::ERROR, "ActuatorCenter : message receptionne de actuatorServerBox");

		if (iTypeMes == 2) // Correspond a un ordre de pilotage
		{
			int iVirtualId = findVirtualId(iMetric, iRoom);
			if (iVirtualId != -1)
			{
				std::cout << "id virtuel trouvŽ : " << iVirtualId << endl;
				GhomeBox::SendMessage(m_iBalModel,iVirtualId, iValue);
				SystemLog::AddLog(SystemLog::SUCCESS, "ActuatorCenter : message transmis a la bal du Model");

			}
			else
			{
				std::cout << "id virtuel non trouvŽ " << endl;
				string log;
				log += "ActuatorCenter : Aucun capteur ne correspond a ces informations : metric : ";
				log += iMetric;
				log += ", room : ";
				log += iRoom;
				SystemLog::AddLog(SystemLog::ERROR, log.c_str());
			}

		}
		else
			SystemLog::AddLog(SystemLog::ERROR, "ActuatorCenter : Le message recu n'est pas un ordre de pilotage. Ce dernier sera ignore.");
	}
}


void ActuatorsCenter::parserXML(const string a_sXmlFile)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(a_sXmlFile.c_str());
	pugi::xml_node xmlActuators = doc.child("actuators");

	if (strcmp(result.description(),"No error")==0)
	{
		SystemLog::AddLog(SystemLog::SUCCESS, "ActuatorCenter : Parsing fichier xml actuators");
		for (pugi::xml_node_iterator actuatorsIt = xmlActuators.begin(); actuatorsIt != xmlActuators.end(); ++actuatorsIt)
		{
			std::pair<std::pair<int,int>,int>  actuatorNode;

			int iVirtualId = actuatorsIt->attribute("id").as_int();
			int iMetricId = atoi(actuatorsIt->child("metric").child_value());
			int iRoomId = atoi(actuatorsIt->child("room").child_value());

			(actuatorNode.first).first = iMetricId;
			(actuatorNode.first).second = iRoomId;
			actuatorNode.second = iVirtualId;

			this->m_pActuators.insert(actuatorNode);
		}
	}
	else
		SystemLog::AddLog(SystemLog::ERROR, "ActuatorCenter : Parsing fichier xml actuators");

}

int ActuatorsCenter::findVirtualId(int a_iMetric, int a_iRoom)
{
	mapActuators::iterator itMapActuators;
	itMapActuators=this->m_pActuators.find(std::pair<int,int> (a_iMetric, a_iRoom));
	int iRes = (itMapActuators != this->m_pActuators.end())? itMapActuators->second : -1;
	return iRes;
}

#ifdef TESTING
mapActuators ActuatorsCenter::GetActuators(){
	return this->m_pActuators;
}

int ActuatorsCenter::GetBalModel(){
	return m_iBalModel;
}

EnOceanActuatorModel* ActuatorsCenter::GetModel(){
	return this->m_pModel;
}
#endif
