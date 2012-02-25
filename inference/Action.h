#ifndef _INFERENCE_ACTION_H
#define _INFERENCE_ACTION_H

//System includes
#include <iostream>

namespace inference
{

class Action
{
public:
	/**
	 * Copy constructor.
	 *
	 * @param action The action instance to copy
	 */
	Action(const Action& action);

	/**
	 * Constructor.
	 *
	 * @param metric The metric managed by the action
	 * @param value The value to apply to the metric
	 */
	Action(int metric, int value);

	/**
	 * Pseudo-comparison operator. The goal of this method is to determine
	 * if the action given as parameter and the current action need to be
	 * merged (i.e. they represent an action on the same thing).
	 *
	 * @param action The action to be compared
	 * @return True if they need to be merged, false otherwise
	 */
	bool equals(const Action& action);

	/**
	 * Merges the action given as parameter into the current action.
	 *
	 * @param action The action to be merged
	 */
	void merge(const Action& action);

	/**
	 * Returns the metric managed by the action.
	 *
	 * @return The metric identifier
	 */
	int getMetric();

	/**
	 * Returns the value to be applied on the metric.
	 *
	 * @return The value to apply
	 */
	int getValue();

	//TODO : Opérateur ==
	bool operator==(const Action &);

private:
	int metric;
	int value;
};

}
#endif /* _INFERENCE_ACTION_H */
