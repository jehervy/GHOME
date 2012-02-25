/*
 * EnOceanSensorModel.h
 *
 *  Created on: 22 janv. 2012
 *      Author: miroof
 */

#ifndef ENOCEANSENSORMODEL_H_
#define ENOCEANSENSORMODEL_H_

#include "AbstractModel.h"
#include <map>
#include <string>

struct SensorInfo{
	enum Type { NUMERIC, BINARY };

	int iVirtualId;
	Type iType;
	int iValid;
	int iPosData;
	int iLengthData;
	int iMin;
	int iMax;
	std::map<int, int> mapValue;
};

typedef std::multimap<std::string, SensorInfo> mapSensorInfo;


class EnOceanSensorModel : public AbstractModel{

public:
	EnOceanSensorModel(int a_iBal);

	virtual ~EnOceanSensorModel();

	virtual void Start();

	virtual void Stop();

private:
	virtual void Run();

	void ParserXml(const char *a_XmlFile);

	mapSensorInfo m_sensorInfo;
	pthread_t m_threadNetwork;
	int m_iBalNetwork;

};

#endif /* ENOCEANSENSORMODEL_H_ */
