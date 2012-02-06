/*
 * sensor_box_reader.cpp
 *
 *  Created on: 22 janv. 2012
 *      Author: Pierre
 */

#include "SensorBoxReader.h"
#include "../Utils/ghome_box.h"
#include <iostream>
using namespace std;
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/errno.h>

void perror(const char *a_pS);


extern int errno ;



SensorBoxReader::~SensorBoxReader() {
	// TODO Auto-generated destructor stub
}


SensorBoxReader::SensorBoxReader(int a_iSensorServerBox, int a_iActuatorServerBox):
		m_iSensorServerBox(a_iSensorServerBox), m_iActuatorServerBox(a_iActuatorServerBox)
{
		//cout << "id boite " << sensorServerBox << endl;
		//cout << "Appel constructeur"<<endl;

		OpenThreadSensorBoxReader();
}



int SensorBoxReader::Run()
{

	int iId, iMetric, iRoom, iValue;
	cout << "Debut du run" << endl;

	GhomeDatabase* logEcriture = new GhomeDatabase("localhost", "root", "mysql", "GHOME");
	logEcriture->OpenDatabase();

	  // message to send (test) TODO : virer
	ghome_box::send_actuator_box(m_iSensorServerBox,2,23,45,12);


		  // read the message from queue

	  for(;;)
	  {
		  ghome_box::receive_message(m_iSensorServerBox, iId, iMetric, iRoom, iValue);

		  if(iId==1) //C'est un message de type information
		  {
		  	// TODO : base de donnee
			  logEcriture->AddTuple("log", 1, 21, 42);

		  } else if(iId==2) { //C'est un message de type pilotage
		  	// TODO : base de donnee et actuatorServerBox
		  ghome_box::send_actuator_box(m_iActuatorServerBox, iId,iMetric,iRoom,iValue);
		  					  }
				//msgctl(sensorServerBox,IPC_RMID,0);
		  }

	//}
		return 0;
}


void *SensorBoxReader::sInit(void * a_pPtr)
{
	SensorBoxReader* pP = (SensorBoxReader*)a_pPtr;
	pP->Run();
	return (0);
}




int SensorBoxReader::OpenThreadSensorBoxReader()
{
	cout << "On est dans open_thread_sensor_box_reader"<<endl;
	int iCheck = pthread_create(&m_threadSensorBoxReader, NULL, &SensorBoxReader::sInit, this);
	return iCheck;
}

void SensorBoxReader::Wait()
{
	pthread_join (m_threadSensorBoxReader, NULL);
}
