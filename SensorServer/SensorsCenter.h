/*
 * SensorsCenter.h
 *
 *  Created on: 22 janv. 2012
 *      Author: miroof
 */

#ifndef SENSORSCENTER_H_
#define SENSORSCENTER_H_

#include <vector>
#include <utility>
#include <pthread.h>
#include "../xml/pugixml.hpp"
#include "EnOceanSensorModel.h"

typedef std::vector<std::pair<int,int> > vectorSensors;

class SensorsCenter{
	public:
		SensorsCenter(int balServer, int balMonitoring, const char* xmlFile);
		virtual ~SensorsCenter();

	private:
		// METHODES
		void parserXML(const char* xmlFile);

		void run();

		static void *callback(void *cxt)
		{
			((SensorsCenter*)cxt)->run();
			return 0;
		}

		// ATTRIBUTS
		int balServer;
		int balMonitoring;
		int balModel;

		pthread_t thread;

		vectorSensors sensors;

		EnOceanSensorModel *model;


};

#endif /* SENSORSCENTER_H_ */
