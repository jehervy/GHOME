/*
 * tests.cpp
 *
 *  Created on: 13 f�vr. 2012
 *      Author: remi
 */

#include "tests.h"






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
	int iFd = papa->GetFd(1);
	test->assert(iFd==130,"El�ment ajout� � la fin du vector");
	iFd = papa->GetFd(2);
	test->assert(iFd==-1,"Indice sup�rieur � la taille du tableau");




}

void ServerPereTests(Test * test)
{
	test->add(&ServerPereTestsSimples, "ServerPereSimple");
}
