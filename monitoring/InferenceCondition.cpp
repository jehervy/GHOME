/*
 * InferenceCondition.cpp
 *
 *  Created on: 16 janv. 2012
 *      Author: vincent
 */

#include "InferenceCondition.h"

InferenceCondition::InferenceCondition(int metric, int comparator, int threshold)
{
	this->metric = metric;
	this->comparator = comparator;
	this->threshold = threshold;
}

InferenceCondition::InferenceCondition(const InferenceCondition& condition)
{
	metric = condition.metric;
	comparator = condition.comparator;
	threshold = condition.threshold;
}

InferenceCondition::~InferenceCondition()
{
}

bool InferenceCondition::match(int metric, int value)
{
	if (this->metric != metric)
	{
		return false;
	}

	switch (comparator)
	{
		case COND_SUP:
			return value > threshold;
		case COND_SUPEQ:
			return value >= threshold;
		case COND_INF:
			return value < threshold;
		case COND_INFEQ:
			return value <= threshold;
		case COND_EQ:
			std::cout << "eq" << std::endl;
			return value == threshold;
	}

	return false;
}
