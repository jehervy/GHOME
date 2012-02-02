/*
 * InferenceAction.cpp
 *
 *  Created on: 16 janv. 2012
 *      Author: vincent
 */

#include "Action.h"
#include <iostream>

namespace inference
{

Action::Action(int metric, int value)
{
	this->metric = metric;
	this->value = value;
}

Action::Action(const Action& action)
{
	metric = action.metric;
	value = action.value;
}

Action::~Action()
{
}

bool Action::equals(const Action& action)
{

	return metric == action.metric;
}

void Action::merge(const Action& action)
{
	if (!equals(action))
	{
		return;
	}

	value = std::max(value, action.value);
}

int Action::getMetric()
{
	return metric;
}

int Action::getValue()
{
	return value;
}

}
