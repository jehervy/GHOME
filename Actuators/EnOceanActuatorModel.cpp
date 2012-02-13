/*
 * EnOceanSensorModel.cpp
 *
 *  Created on: 22 janv. 2012
 *      Author: miroof
 */

#include "../Utils/DataContext.h"
#include "EnOceanActuatorModel.h"
#include "../Utils/GhomeBox.h"
#include <iostream>
#include <algorithm>
#include <sys/ipc.h>
#include <sys/msg.h>	//pour la boite aux lettres
#include <cstring>
#include <string>
#include <cstdlib>

using namespace std;

#define DROITS 0660 	// Droits d'accès
#define REFERENCE "." 	//Fichier courant utilisé pour bâtir la clé publique

EnOceanActuatorModel::EnOceanActuatorModel(int a_iBal) : AbstractModel(a_iBal)
{
	m_iBalNetwork = msgget (ftok (REFERENCE, '3'), IPC_CREAT | DROITS );
	if(m_iBalNetwork == -1)
		SystemLog::AddLog(SystemLog::ERROR, "ActuatorModel : Reception message BalNetwork");
	else SystemLog::AddLog(SystemLog::SUCCES, "ActuatorModel : Reception message BalNetwork");

	parserXml("etc/enOceanActuatorsId.xml");
}

void EnOceanActuatorModel::parserXml(string a_sXmlFile)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(a_sXmlFile.c_str());
	pugi::xml_node xmlActuators = doc.child("actuators");

	if (strcmp(result.description(),"No error")==0)
		{
		SystemLog::AddLog(SystemLog::SUCCES, "ActuatorModel : Parsing fichier xml actuatorsId");
		for (pugi::xml_node_iterator actuatorsIt = xmlActuators.begin(); actuatorsIt != xmlActuators.end(); ++actuatorsIt)
		{
			if (strcmp(actuatorsIt->name(), "entete") == 0)
				m_psEntete = actuatorsIt->child_value();
			if (strcmp(actuatorsIt->name(), "activate") == 0)
				m_psActivate = actuatorsIt->child_value();
			if (strcmp(actuatorsIt->name(), "desactivate") == 0)
				m_psDesactivate = actuatorsIt->child_value();
			if (strcmp(actuatorsIt->name(), "status") == 0)
				m_psStatus = actuatorsIt->child_value();
			if (strcmp(actuatorsIt->name(), "checksum") == 0)
				m_psChecksum = actuatorsIt->child_value();
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

		GhomeBox::ReceiveMessage(m_iBalCenter, iVirtualId, iValue);
		cout << "Message recu : " << iVirtualId << " " << iValue << endl;

		sPhysicalId = findId(iVirtualId);

		createOrder(sPhysicalId, iValue, msg);

		cout << "Sending order " << msg.mtext << " to " << sPhysicalId << endl;
		msgsnd(m_iBalNetwork, &msg, MSGSIZE, 0);
	}
}

void EnOceanActuatorModel::start()
{
	AbstractModel::Start();

	networkInfo *infos = (networkInfo *)malloc(sizeof(networkInfo));
	strcpy(infos->address, "localhost");
	infos->port = 5000;
	infos->bal = m_iBalNetwork;

	pthread_create(&m_thread, NULL, DataContext::sSndData, (void *)infos);
}

void EnOceanActuatorModel::stop()
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
	if (sRes.length() == 0)
		cout << "cet id virtuel :" << a_iVirtualId << "ne correspond � aucun id physique enregistr�." << endl;
	return sRes;
}

void EnOceanActuatorModel::createOrder(string a_sPhysicalId, int iValue, balMessage &res)
{
	res.mtype = 1;
	string message;
	// Construction de l'ent�te
	strcat(res.mtext, m_psEntete.c_str());
	// Remplissage des databytes selon l'ordre de pilotage
	if (iValue == 0)
		strcat(res.mtext, m_psDesactivate.c_str());
	if (iValue == 1)
		strcat(res.mtext, m_psActivate.c_str());

	// Remplissage de l'id de destination
	strcat(res.mtext, a_sPhysicalId.c_str());

	// Status et checksum
	strcat(res.mtext, m_psStatus.c_str());
	strcat(res.mtext, m_psChecksum.c_str());
}
