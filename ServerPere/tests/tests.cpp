/*
 * tests.cpp
 *
 *  Created on: 13 f�vr. 2012
 *      Author: remi et Pierre
 */

#include "tests.h"
#include "SensorBoxReader.h"





void ServerPereTestsSimples(Test * test)
{


	cout<<"Phase de test simple sur le serveur"<<endl;
	ServerPere * papa = new ServerPere();
	vector<int> m_vVectorFd;
	int iTailleVect=papa->InsertFd(120);
	test->assert(iTailleVect==1,"Ajout d'un �l�ment au vector");
	iTailleVect=papa->DeleteFd(112);
	test->assert(iTailleVect==1,"El�ment absent du vector");
	iTailleVect=papa->InsertFd(130);
	test->assert(iTailleVect==2,"Ajoute element a la fin du vector")
	iTailleVect = papa->DeleteFd(130);
	test->assert(iTailleVect==1,"El�ment supprime du tableau");


}

void SensorBoxReader(Test *test)
{
	int sensorServerBoxTest = msgget (IPC_PRIVATE, IPC_CREAT | DROITS );
	int actuatorServerBoxTest = msgget (IPC_PRIVATE, IPC_CREAT | DROITS );
	SensorBoxReader * box = new SensorBoxReader(sensorServerBoxTest, actuatorServerBoxTest);
	test->assert( box->OpenThreadSensorBoxReader() >= 0, "Création d'un thread de lecture des messages des capteurs");


}

void ServerPereTests(Test * test)
{
	test->add(&ServerPereTestsSimples, "ServerPereSimple");
	test->add(&SensorBoxReader, "SensorBoxReader");
}
