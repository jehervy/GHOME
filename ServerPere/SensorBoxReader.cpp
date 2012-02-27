/*
 * sensor_box_reader.cpp
 *
 *  Created on: 22 janv. 2012
 *      Author: Pierre
 */

#include <iostream>
using namespace std;
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include <sstream>

#include "SensorBoxReader.h"
#include "../Utils/GhomeBox.h"
#include "../Utils/SystemLog.h"

//void perror(const char *a_pS);


extern int errno ;



SensorBoxReader::~SensorBoxReader() {
	// TODO Auto-generated destructor stub
}


SensorBoxReader::SensorBoxReader(int a_iSensorServerBox, int a_iActuatorServerBox):
		m_iSensorServerBox(a_iSensorServerBox), m_iActuatorServerBox(a_iActuatorServerBox)
{
		//cout << "id boite " << sensorServerBox << endl;
		//cout << "Appel constructeur"<<endl;


		int retour = OpenThreadSensorBoxReader();

		if (retour < 0)
		{
			std::ostringstream oss;
			oss << "Création du thread de lecture des messages des capteurs, retour : " << retour ;
			SystemLog::AddLog(SystemLog::ERROR, oss.str());
		}
		else
		{
			SystemLog::AddLog(SystemLog::SUCCESS, "Création du thread de lecture des messages des capteurs");
		}
}



int SensorBoxReader::Run()
{

	int iId, iMetric, iRoom, iValue, iReturn, bReturn;

	GhomeDatabase* logEcriture = new GhomeDatabase("localhost", "boby", "ghome", "GHOME");
	iReturn = logEcriture->OpenDatabase();
	if (iReturn == 0)
	{
		SystemLog::AddLog(SystemLog::SUCCESS, "Ouverture de la base de données");
	}
	else
	{
		std::ostringstream oss;
		oss << "ouverture de la base de données, retour : " << iReturn ;
		SystemLog::AddLog(SystemLog::ERROR, oss.str());
	}
	// read the message from queue
	for(;;)
	{
		bReturn = GhomeBox::ReceiveMessage(m_iSensorServerBox, iId, iMetric, iRoom, iValue);
		if (bReturn)
		{
			SystemLog::AddLog(SystemLog::SUCCESS, "Lecture d'un message capteur");
		}
		else
		{
			std::ostringstream oss;
			oss << "Lecture d'un message capteur, retour : " << iReturn ;
			SystemLog::AddLog(SystemLog::ERROR, oss.str());

		}

		  if(iId==1) //C'est un message de type information
		  {
		  	// TODO : base de donnee
			  iReturn = logEcriture->AddTuple("sensors_values", iRoom, iMetric, iValue);
			  if( iReturn == 0)
			  {
				  SystemLog::AddLog(SystemLog::SUCCESS, "Ecriture d'une valeur de capteur en BDD");

			  }
			  else
			  {
				  std::ostringstream oss;
				  oss << "Ecriture d'une valeur de capteur en BDD, retour : " << iReturn ;
				  SystemLog::AddLog(SystemLog::ERROR, oss.str());

			  }
		  }
		  else if(iId==2)
		  { //C'est un message de type pilotage
		  	// TODO : base de donnee et actuatorServerBox


			  iReturn = logEcriture->AddTuple("actuators_commands", "System",iRoom, iMetric, iValue);
			  if( iReturn == 0)
			  {
				  SystemLog::AddLog(SystemLog::SUCCESS, "Ecriture d'un ordre d'actionneur en BDD");

			  }
			  else
			  {
				  std::ostringstream oss;
				  oss << "Ecriture d'un ordre d'actionneur en BDD, retour : " << iReturn ;
				  SystemLog::AddLog(SystemLog::ERROR, oss.str());

			  }
			  bReturn =  GhomeBox::SendActuatorBox(m_iActuatorServerBox, iId,iMetric,iRoom,iValue);
			  if (bReturn)
				{
					SystemLog::AddLog(SystemLog::SUCCESS, "Envoi d'un ordre à un actionneur");
				}
				else
				{
					std::ostringstream oss;
					oss << "Envoi d'un ordre à un actionneur, retour : " << bReturn ;
					SystemLog::AddLog(SystemLog::ERROR, oss.str());

				}
		  }

	}
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
	int iCheck = pthread_create(&m_threadSensorBoxReader, NULL, &SensorBoxReader::sInit, this);
	if (iCheck < 0)
	{
		std::ostringstream oss;
		oss << "Création d'un thread de lecture des messages des capteurs, retour : " << iCheck ;
		SystemLog::AddLog(SystemLog::ERROR, oss.str());
	}
	else
	{
		SystemLog::AddLog(SystemLog::SUCCESS, "Création d'un thread de lecture des messages des capteurs");
	}
	return iCheck;
}

void SensorBoxReader::Wait()
{
	pthread_join (m_threadSensorBoxReader, NULL);
	SystemLog::AddLog(SystemLog::SUCCESS, "Attente sur le thread de lecture des messages des capteurs");
}
