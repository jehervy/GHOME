/*
 * InferenceRule.cpp
 *
 *  Created on: 16 janv. 2012
 *      Author: vincent
 */

#include "InferenceRule.h"

InferenceRule::InferenceRule()
{
}

InferenceRule::~InferenceRule()
{
}

bool InferenceRule::match(int metric, int value)
{
	for (std::vector<InferenceCondition>::iterator it = conditions.begin(); it != conditions.end(); ++it)
	{
		if (!it->match(metric, value))
		{
			return false;
		}
	}

	return true;
}

void InferenceRule::addCondition(int metric, char* comparator, int threshold)
{
	int intComparator;

	if (!strcmp(comparator, "sup"))
	{
		intComparator = InferenceCondition::COND_SUP;
	}
	else if (!strcmp(comparator, "supeq"))
	{
		intComparator = InferenceCondition::COND_SUPEQ;
	}
	else if (!strcmp(comparator, "inf"))
	{
		intComparator = InferenceCondition::COND_INF;
	}
	else if (!strcmp(comparator, "infeq"))
	{
		intComparator = InferenceCondition::COND_INFEQ;
	}
	else if (!strcmp(comparator, "ep"))
	{
		intComparator = InferenceCondition::COND_EQ;
	}
	else
	{
		return;
	}

	InferenceCondition condition(metric, intComparator, threshold);
	conditions.push_back(condition);
}

void InferenceRule::addAction(int metric, int value)
{
	InferenceAction action(metric, value);
	actions.push_back(action);
}

InferenceActions InferenceRule::getActions()
{
	return actions;
}
