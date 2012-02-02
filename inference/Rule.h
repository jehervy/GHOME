/*
 * InferenceRule.h
 *
 *  Created on: 16 janv. 2012
 *      Author: vincent
 */

#ifndef _INFERENCE_RULE_H
#define _INFERENCE_RULE_H

#include <vector>
#include <iostream>

#include "typedefs.h"
#include "Action.h"

namespace inference
{

class Rule
{
public:
	Rule();
	Rule(const Rule& rule);

	bool match(std::map <int, int> state);
	void addCondition(int metric, char* comparator, int threshold);
	void addAction(int metric, int value);
	std::vector<Action> getActions();
protected:
	std::vector<Condition> conditions;
	std::vector<Action> actions;
};

}
#endif /* _INFERENCE_RULE_H */
