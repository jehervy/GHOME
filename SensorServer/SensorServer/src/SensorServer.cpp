//============================================================================
// Name        : SensorServer.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "SensorData.h"
using namespace std;

int main() {
	SensorData *data = new SensorData();
	data->addSensor("sensor1");
	data->addSensor("sensor2");
	data->addSensor("sensor3");
	data->updateData("sensor1", "data1");
	data->updateData("sensor2", "data2");
	data->updateData("sensor3", "data3");
	cout << data->getData("sensor2") << endl;
	cout << data->getData("sensor1") << endl;
	cout << data->getData("sensor3") << endl << endl;

	data->updateData("sensor1", "data1bis");
	cout << data->getData("sensor1") << endl;
}
