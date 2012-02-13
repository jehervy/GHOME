/*
 * tests.cpp
 *
 *  Created on: 13 fvr. 2012
 *      Author: remi
 */

#include "tests.h"





void ServerPereTestsSimples(Test * test)
{
	cout<<"Phase de test simple"<<endl;
	cout<<"Creation de l'objet ServerPere"<<endl;
	ServerPere * papa = new ServerPere(100,200);
	cout<<"Ouverture du socket du ServerPere"<<endl;
	papa->OpenSocket();
	//test->assert(papa->m_bSocketOpened=true,"Socket en ecoute sur un thread");


}

void ServerPereTests(Test * test)
{
	test->add(&ServerPereTestsSimples, "ServerPereSimple");
}
