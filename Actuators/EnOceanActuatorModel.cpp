/*
 * EnOceanSensorModel.cpp
 *
 *  Created on: 22 janv. 2012
 *      Author: Xav
 */

#include "../Utils/DataContext.h"
#include "EnOceanActuatorModel.h"
#include "../Utils/GhomeBox.h"
#include "../Utils/SystemLog.h"
#include <iostream>
#include <algorithm>
#include <sys/ipc.h>
#include <sys/msg.h>	//pour la boite aux lettres
#include <cstring>
#include <string>
#include <cstdlib>
#include <cstdio>

using namespace std;

#define DROITS 0660 	// Droits d'acces
#define REFERENCE "." 	//Fichier courant utilise pour batir la cle publique

EnOceanActuatorModel::EnOceanActuatorModel(int a_iBal) : AbstractModel(a_iBal)
{

	m_iBalNetwork = msgget (IPC_PRIVATE, IPC_CREAT | DROITS );
	if(m_iBalNetwork == -1)
		SystemLog::AddLog(SystemLog::ERROR, "ActuatorModel : Reception message BalNetwork");
	else SystemLog::AddLog(SystemLog::SUCCESS, "ActuatorModel : Reception message BalNetwork");

	parserXml("etc/enOceanActuatorsId.xml");
}

EnOceanActuatorModel::~EnOceanActuatorModel()
{

}

void EnOceanActuatorModel::parserXml(string a_sXmlFile)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(a_sXmlFile.c_str());
	pugi::xml_node xmlActuators = doc.child("actuators");

	if (strcmp(result.description(),"No error")==0)
		{
		SystemLog::AddLog(SystemLog::SUCCESS, "ActuatorModel : Parsing fichier xml actuatorsId");
		for (pugi::xml_node_iterator actuatorsIt = xmlActuators.begin(); actuatorsIt != xmlActuators.end(); ++actuatorsIt)
		{
			if (strcmp(actuatorsIt->name(), "entete") == 0)
				(this->p_myInfoTrame).m_psEntete = actuatorsIt->child_value();
			if (strcmp(actuatorsIt->name(), "activate") == 0)
				(this->p_myInfoTrame).m_psActivate = actuatorsIt->child_value();
			if (strcmp(actuatorsIt->name(), "desactivate") == 0)
				(this->p_myInfoTrame).m_psDesactivate = actuatorsIt->child_value();
			if (strcmp(actuatorsIt->name(), "status") == 0)
				(this->p_myInfoTrame).m_psStatus = actuatorsIt->child_value();
			if (strcmp(actuatorsIt->name(), "checksum") == 0)
				(this->p_myInfoTrame).m_psChecksum = actuatorsIt->child_value();
			if (strcmp(actuatorsIt->name(), "actuator") == 0)
			{
				int iVirtualId = atoi(actuatorsIt->child("virtualId").child_value());
				const string sPhysicalId = actuatorsIt->child("physicalId").child_value();
				this->m_actuatorsId.insert(pair<int,const string> (iVirtualId,sPhysicalId));
			}
		}
		}
	else
		SystemLog::AddLog(SystemLog::ERROR, "ActuatorModel : Parsing fichier xml actuatorsId");

}

void EnOceanActuatorModel::Run()
{
	balMessage msg;
	while(true)
	{
		int iVirtualId, iValue;
		string sPhysicalId;

		strcpy(msg.mtext, "");

		GhomeBox::ReceiveMessage(m_iBalCenter, iVirtualId, iValue);

		sPhysicalId = findId(iVirtualId);

		if (sPhysicalId != "")
		{
			createOrder(sPhysicalId, iValue, msg);
			msgsnd(m_iBalNetwork, &msg, MSGSIZE, 0);
			SystemLog::AddLog(SystemLog::SUCCESS, "ActuatorModel : Transmission de l'ordre de pilotage ˆ l'actionneur");
		}
		else
		{
			SystemLog::AddLog(SystemLog::ERROR, "Transmission de l'ordre de pilotage ˆ l'actionneur");
		}

#ifdef TESTING
		orderSent = msg.mtext;
#endif
	}
}

void EnOceanActuatorModel::Start()
{
	networkInfo *infos = (networkInfo *)malloc(sizeof(networkInfo));
	strcpy(infos->address, "localhost");
	infos->port = 5000;
	infos->bal = m_iBalNetwork;

	pthread_create(&m_thread, NULL, DataContext::sSndData, (void *)infos);
	AbstractModel::Start();

}

void EnOceanActuatorModel::Stop()
{
	pthread_cancel(m_threadNetwork);
	AbstractModel::Stop();
	msgctl(m_iBalNetwork,IPC_RMID,0);
}

string EnOceanActuatorModel::findId(int a_iVirtualId){
	string sRes;
	mapActuatorsId::iterator itActuatorsId;

	itActuatorsId = this->m_actuatorsId.find(a_iVirtualId);
	sRes = (itActuatorsId!=this->m_actuatorsId.end())? itActuatorsId->second : "";
	if (sRes.length() != 0)
		return sRes;
	else return "";
}

void EnOceanActuatorModel::createOrder(string a_sPhysicalId, int iValue, balMessage &res)
{
	res.mtype = 1;
	string message;
	// Construction de l'entete
	strcat(res.mtext, (this->p_myInfoTrame).m_psEntete.c_str());
	// Remplissage des databytes selon l'ordre de pilotage
	if (iValue == 0)
		strcat(res.mtext, (this->p_myInfoTrame).m_psDesactivate.c_str());
	if (iValue == 1)
		strcat(res.mtext, (this->p_myInfoTrame).m_psActivate.c_str());

	// Remplissage de l'id de destination
	strcat(res.mtext, a_sPhysicalId.c_str());

	// Status et checksum
	strcat(res.mtext, (this->p_myInfoTrame).m_psStatus.c_str());
	strcat(res.mtext, (this->p_myInfoTrame).m_psChecksum.c_str());
}

#ifdef TESTING

mapActuatorsId EnOceanActuatorModel::GetActuatorsId(){
	return m_actuatorsId;
}

infoTrame EnOceanActuatorModel::GetInfoTrame(){
	return this->p_myInfoTrame;
}

void EnOceanActuatorModel::SetBalNetwork(int *balId)
{
	this->m_iBalNetwork = *balId;
}
#endif
