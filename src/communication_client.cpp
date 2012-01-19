/*
 * communicationclient.cpp
 *
 *  Created on: 17 janv. 2012
 *      Author: remi
 */

#include "communication_client.h"

communication_client::communication_client() {
	cout << "Apero" << endl;

}

communication_client::communication_client(int sensorServerBox, int actuatorServerBox) :
p_sensorServerBox(sensorServerBox), p_actuatorServerBox(actuatorServerBox){


}

communication_client::~communication_client() {

}



