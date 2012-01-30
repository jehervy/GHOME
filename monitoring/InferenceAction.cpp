/*
 * InferenceAction.cpp
 *
 *  Created on: 16 janv. 2012
 *      Author: vincent
 */

#include "InferenceAction.h"
#include <iostream>

InferenceAction::InferenceAction(int metric, int value)
{
	this->metric = metric;
	this->value = value;
}

InferenceAction::InferenceAction(const InferenceAction& action)
{
	metric = action.metric;
	value = action.value;
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
