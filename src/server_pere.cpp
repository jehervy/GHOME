/*
 * serverpere.cpp
 *
 *  Created on: 16 janv. 2012
 *      Author: remi
 */

#include "server_pere.h"
#include <iostream>
using namespace std;
#include <pthread.h>


void print_port(int p_port_ecoute)
{
	cout << "On Žcoute sur le port : ";
	cout <<p_port_ecoute << endl;
}

server_pere::~server_pere() {
	// TODO Auto-generated destructor stub
}


server_pere::server_pere(int port_ecoute) : p_port_ecoute(port_ecoute) {
	print_port(p_port_ecoute);


}

