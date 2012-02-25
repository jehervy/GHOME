#ifndef _INFERENCE_RULE_H
#define _INFERENCE_RULE_H

//System includes
#include <vector>
#include <string>
#include <iostream>

//Personal includes
#include "types.h"
#include "Action.h"

namespace inference
{

class Rule
{
public:
	/**
	 * Default constructor.
	 */
	Rule() { }

	/**
	 * Copy constructor.
	 *
	 * @param rule The rule instance to copy
	 */
	Rule(const Rule& rule);

	/**
	 * Checks if a given state matches with this rule.
	 *
	 * @param state The state to test
	 */
	bool match(State state);

	/**
	 * Adds a new condition to match for the rule.
	 *
	 * @param metric The metric to consider
	 * @param comparator The comparator to use
	 * @param threshold The threshold to compare with
	 */
	void addCondition(int metric, std::string comparator, int threshold);

	/**
	 * Adds a new action to do when matched for the rule.
	 *
	 * @param metric The metric to manage
	 * @param value The value to apply
	 */
	void addAction(int metric, int value);

	/**
	 * Returns the actions set to do when the rule is matched.
	 *
	 * @return Action to do
	 */
	Actions getActions();

protected:
	Conditions conditions;
	Actions actions;
};

}
#endif /* _INFERENCE_RULE_H */
