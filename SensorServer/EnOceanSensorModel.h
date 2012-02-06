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
#include <cstring>

struct StrCompare : public std::binary_function<char*, char*, bool> {
public:
    bool operator() (char* str1, char* str2) const
    { return std::strcmp(str1, str2) < 0; }
};


struct SensorInfo{
	int virtualId;
};

typedef std::multimap<char*, SensorInfo, StrCompare> mapSensorInfo;


class EnOceanSensorModel : public AbstractModel{

public:
	EnOceanSensorModel(int a_iBal);

	virtual ~EnOceanSensorModel();

	virtual void Start();

	virtual void Stop();

private:
	virtual void Run();

	void ParserXml(const char *a_XmlFile);

	char* GetId(char a_Data[29]);

	char* GetData(char a_Data[29]);

	mapSensorInfo m_sensorInfo;
	pthread_t m_threadNetwork;
	int m_iBalNetwork;

};

#endif /* ENOCEANSENSORMODEL_H_ */
