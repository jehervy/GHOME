/*
 * Rule.cpp
 *
 *  Created on: 16 janv. 2012
 *      Author: vincent
 */

#include "Rule.h"
#include <string.h>

inference::Rule::Rule()
{
}

inference::Rule::Rule(const inference::Rule& rule)
{
	conditions = rule.conditions;
	actions = rule.actions;
}


bool inference::Rule::match(inference::State state)
{
	for (inference::Conditions::iterator it = conditions.begin(); it != conditions.end(); ++it)
	{
		bool match = false;
		for (inference::State::iterator stateIt = state.begin(); stateIt != state.end(); stateIt++)
		{
			if (it->match(stateIt->first, stateIt->second))
			{
				match = true;
				break;
			}
		}

		if (!match)
		{
			return false;
		}
	}

	return true;
}

void inference::Rule::addCondition(int metric, char* comparator, int threshold)
{
	int intComparator;

	if (!strcmp(comparator, "sup"))
	{
		intComparator = inference::Condition::COND_SUP;
	}
	else if (!strcmp(comparator, "supeq"))
	{
		intComparator = inference::Condition::COND_SUPEQ;
	}
	else if (!strcmp(comparator, "inf"))
	{
		intComparator = inference::Condition::COND_INF;
	}
	else if (!strcmp(comparator, "infeq"))
	{
		intComparator = inference::Condition::COND_INFEQ;
	}
	else if (!strcmp(comparator, "ep"))
	{
		intComparator = inference::Condition::COND_EQ;
	}
	else
	{
		return;
	}

	inference::Condition condition(metric, intComparator, threshold);
	conditions.push_back(condition);
}

void inference::Rule::addAction(int metric, int value)
{
	inference::Action action(metric, value);
	actions.push_back(action);
}

inference::Actions inference::Rule::getActions()
{
	return actions;
}

