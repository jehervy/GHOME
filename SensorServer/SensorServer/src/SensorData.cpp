/*
 * SensorData.cpp
 *
 *  Created on: 9 janv. 2012
 *      Author: miroof
 */

#include <iostream>
#include <string>
#include "SensorData.h"

using namespace std;



SensorData::SensorData()
{
	head = NULL;
	size = 0;
}

void SensorData::addSensor(string id)
{
	SensorDataElement *elemTmp = new SensorDataElement();
	elemTmp->id = id;
	elemTmp->nextElem = NULL;
	elemTmp->data = "";

	if(head == NULL)
	{
		head = elemTmp;
	}
	else
	{
		SensorDataElement *elemTmp2 = head;
		while(head->nextElem != NULL)
		{
			head = head->nextElem;
		}
		head->nextElem = elemTmp;
		head = elemTmp2;
	}
	size ++;
}

int SensorData::getSize()
{
	return size;
}

bool SensorData::updateData(string id, string data)
{
	SensorDataElement *elemTmp = head;
	while(head != NULL)
	{
		if(head->id == id)
		{
			head->data = data;
			head = elemTmp;
			return true;
		}
		head = head->nextElem;
	}
	head = elemTmp;
	return false;
}

string SensorData::getData(string id)
{
	SensorDataElement *elemTmp = head;
	while(elemTmp != NULL)
	{
		if(elemTmp->id == id)
		{
			return elemTmp->data;
		}
		elemTmp = elemTmp->nextElem;
	}
	cout << "data not found" << endl;
	return NULL;
}

