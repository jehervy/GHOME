/*
 * SensorsCenter.h
 *
 *  Created on: 22 janv. 2012
 *      Author: miroof
 */

#ifndef SENSORSCENTER_H_
#define SENSORSCENTER_H_

#include <map>
#include <utility>
#include <pthread.h>
#include "EnOceanSensorModel.h"

typedef std::map<int, std::pair<int,int> > mapSensors;

class SensorsCenter{
	public:
		SensorsCenter(int a_iBalServer, int a_iBalMonitoring, const char* a_pXmlFile);
		virtual ~SensorsCenter();

		void Start();
		void Stop();

	private:
		// METHODES
		void ParserXML(const char* a_pXmlFile);

		void Run();

		static void *sCallback(void *a_pCxt)
		{
			((SensorsCenter*)a_pCxt)->Run();
			return 0;
		}

		// ATTRIBUTS
		int m_iBalServer;
		int m_iBalMonitoring;
		int m_iBalModel;

		pthread_t m_thread;
		mapSensors m_sensors;
		EnOceanSensorModel *m_pModel;


};

#endif /* SENSORSCENTER_H_ */
