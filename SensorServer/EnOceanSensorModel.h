/*
 * EnOceanSensorModel.h
 *
 *  Created on: 22 janv. 2012
 *      Author: miroof
 */

#ifndef ENOCEANSENSORMODEL_H_
#define ENOCEANSENSORMODEL_H_

#include "AbstractModel.h"

class EnOceanSensorModel : public AbstractModel{

public:
	EnOceanSensorModel(int bal);

	virtual void start();

	virtual void stop();

private:
	virtual void run();

	pthread_t threadNetwork;
	int balNetwork;





};

#endif /* ENOCEANSENSORMODEL_H_ */
