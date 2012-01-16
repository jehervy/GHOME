/*
 * InferenceAction.cpp
 *
 *  Created on: 16 janv. 2012
 *      Author: vincent
 */

#include "InferenceAction.h"

InferenceAction::InferenceAction(int metric, int value)
{
	this->metric = metric;
	this->value = value;
}

InferenceAction::~InferenceAction()
{
}

int InferenceAction::getMetric()
{
	return metric;
}

int InferenceAction::getValue()
{
	return value;
}
