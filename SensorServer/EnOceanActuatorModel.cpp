/*
 * EnOceanSensorModel.cpp
 *
 *  Created on: 22 janv. 2012
 *      Author: miroof
 */

#include "DataContext.h"
#include "EnOceanActuatorModel.h"
#include "../Utils/ghome_box.h"
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
	parserXml("src/etc/enOceanActuatorsId.xml");
}

void EnOceanActuatorModel::parserXml(string a_sXmlFile)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(a_sXmlFile.c_str());
	pugi::xml_node xmlActuators = doc.child("actuators");
	cout << "Load result: " << result.description() << endl;

	for (pugi::xml_node_iterator actuatorsIt = xmlActuators.begin(); actuatorsIt != xmlActuators.end(); ++actuatorsIt)
	{
		int iVirtualId = atoi(actuatorsIt->child("virtualId").child_value());
		const string sPhysicalId = actuatorsIt->child("physicalId").child_value();
		this->m_actuatorsId.insert(pair<int,const string> (iVirtualId,sPhysicalId));
	}
}

void EnOceanActuatorModel::run()
{
	cout << "RUN EnOceanActuatorModel" << endl;
	balMessage msg;
	while(true)
	{
		int iVirtualId, iValue;
		string sPhysicalId;

		cout << "balcenter du modele : " << balCenter << endl;
		ghome_box::receive_message(balCenter, iVirtualId, iValue);

		cout<<"j'ai recu un message : " << iVirtualId << " " << iValue << endl;

		sPhysicalId = findId(iVirtualId);

		cout<<"id physique de l'actionneur : " << sPhysicalId << endl;

		msg = createOrder(sPhysicalId, iValue);

		msgsnd(m_iBalNetwork, &msg, MSGSIZE, 0);
	}
}

void EnOceanActuatorModel::start()
{
	AbstractModel::start();

	networkInfo *infos = (networkInfo *)malloc(sizeof(networkInfo));
	strcpy(infos->address, "localhost");
	infos->port = 5000;
	infos->bal = m_iBalNetwork;

	pthread_create(&thread, NULL, DataContext::sndData, (void *)infos);
}

void EnOceanActuatorModel::stop()
{
	pthread_cancel(m_threadNetwork);
	AbstractModel::stop();
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

balMessage EnOceanActuatorModel::createOrder(string a_sPhysicalId, int iValue)
{
	balMessage res;

	return res;
}
