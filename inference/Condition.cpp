//Personal includes
#include "Condition.h"

/**
 * Trivial.
 */
inference::Condition::Condition(int metric, int comparator, int threshold)
{
	this->metric = metric;
	this->comparator = comparator;
	this->threshold = threshold;
}

/**
 * Trivial.
 */
inference::Condition::Condition(const inference::Condition& condition)
{
	metric = condition.metric;
	comparator = condition.comparator;
	threshold = condition.threshold;
}

/**
 * A metric and value couple match if the metric is the same as the
 * condition's metric and the value is consistent with the comparator
 * and threshold of the condition.
 */
bool inference::Condition::match(int metric, int value)
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
			return value == threshold;
	}

	return false;
}

