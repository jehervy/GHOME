//Personal includes
#include "Rule.h"

/**
 * Trivial.
 */
inference::Rule::Rule(const inference::Rule& rule)
{
	conditions = rule.conditions;
	actions = rule.actions;
}

/**
 * For each metric and value couple in the state given as parameter, at least
 * one condition must match for the entire rule to match.
 */
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

/**
 * Creates a new Condition instance with the given parameters, after having
 * translated the string comparator to an integer constant.
 */
void inference::Rule::addCondition(int metric, std::string comparator, int threshold)
{
	int intComparator;

	if (comparator == "sup")
	{
		intComparator = inference::Condition::COND_SUP;
	}
	else if (comparator == "supeq")
	{
		intComparator = inference::Condition::COND_SUPEQ;
	}
	else if (comparator == "inf")
	{
		intComparator = inference::Condition::COND_INF;
	}
	else if (comparator == "infeq")
	{
		intComparator = inference::Condition::COND_INFEQ;
	}
	else if (comparator == "eq")
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

/**
 * Creates a new Action instance with the given parameters.
 */
void inference::Rule::addAction(int metric, int value)
{
	inference::Action action(metric, value);
	actions.push_back(action);
}

/**
 * Trivial.
 */
inference::Actions inference::Rule::getActions()
{
	return actions;
}

