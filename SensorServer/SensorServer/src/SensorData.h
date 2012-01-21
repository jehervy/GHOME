/*
 * SensorData.h
 *
 *  Created on: 9 janv. 2012
 *      Author: miroof
 */

#ifndef SENSORDATA_H_
#define SENSORDATA_H_

class SensorDataElement
{
public:
	std::string id;
	std::string data;
	SensorDataElement *nextElem;
};

class SensorData
{
private:
	SensorDataElement *head;
	int size;

public:
	SensorData();

	void addSensor(std::string id);

	int getSize();

	bool updateData(std::string id, std::string data);

	std::string getData(std::string id);

};

#endif /* SENSORDATA_H_ */
