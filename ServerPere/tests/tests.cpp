/*
 * tests.cpp
 *
 *  Created on: 13 f�vr. 2012
 *      Author: remi
 */

#include "tests.h"






void ServerPereTestsSimples(Test * test)
{
	int sensorServerBox = msgget (IPC_PRIVATE, IPC_CREAT | DROITS );
	int actuatorServerBox = msgget (IPC_PRIVATE, IPC_CREAT | DROITS );

	cout<<"Phase de test simple"<<endl;
	cout<<"Creation de l'objet ServerPere"<<endl;
	ServerPere * papa = new ServerPere(sensorServerBox,actuatorServerBox);
	cout<<"Ouverture du socket du ServerPere"<<endl;
	//papa->Start();
	papa->OpenSocket();
	//test->assert(papa->m_bSocketOpened=true,"Socket en ecoute sur un thread");


}

void ServerPereTests(Test * test)
{
	test->add(&ServerPereTestsSimples, "ServerPereSimple");
}
