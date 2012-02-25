//Personal includes
#include "Action.h"

/**
 * Trivial.
 */
inference::Action::Action(int metric, int value)
{
	this->metric = metric;
	this->value = value;
}

/**
 * Trivial.
 */
inference::Action::Action(const inference::Action& action)
{
	metric = action.metric;
	value = action.value;
}

/**
 * Two actions need to be merged if they manage the same metric.
 */
bool inference::Action::equals(const inference::Action& action)
{

	return metric == action.metric;
}

/**
 * TODO change the max strategy to a more appropriate one.
 */
void inference::Action::merge(const inference::Action& action)
{
	if (!equals(action))
	{
		return;
	}

	value = std::max(value, action.value);
}

/**
 * Trivial.
 */
int inference::Action::getMetric()
{
	return metric;
}

/**
 * Trivial.
 */
int inference::Action::getValue()
{
	return value;
}


bool inference::Action::operator ==(const inference::Action &action)
{
	return(	(this->metric == action.metric) &&
			(this->value == action.value));
}
