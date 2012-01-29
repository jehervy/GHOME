/*
 * EnOceanSensorModel.h
 *
 *  Created on: 22 janv. 2012
 *      Author: miroof
 */

#ifndef ENOCEANSENSORMODEL_H_
#define ENOCEANSENSORMODEL_H_

#include "AbstractModel.h"
#include <vector>
#include <utility>
#include "../xml/pugixml.hpp"

typedef std::vector<std::pair<int,const char*> > vectorSensorsId;

class EnOceanSensorModel : public AbstractModel{

public:
	EnOceanSensorModel(int bal);

	virtual void start();

	virtual void stop();

private:
	virtual void run();

	void parserXml(const char *xmlFile);

	char* get_id(char data[29]);

	char* get_data(char data[29]);

	vectorSensorsId sensorsId;
	pthread_t threadNetwork;
	int balNetwork;

};

#endif /* ENOCEANSENSORMODEL_H_ */
