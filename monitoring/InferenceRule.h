/*
 * InferenceRule.h
 *
 *  Created on: 16 janv. 2012
 *      Author: vincent
 */

#ifndef _INFERENCERULE_H
#define _INFERENCERULE_H

#include <vector>
#include <iostream>

#include "InferenceCondition.h"
#include "InferenceAction.h"
#include "typedefs.h"

class InferenceRule {
public:
	InferenceRule();
	InferenceRule(const InferenceRule& rule);
	virtual ~InferenceRule();
	bool match(State state);
	void addCondition(int metric, char* comparator, int threshold);
	void addAction(int metric, int value);
	InferenceActions getActions();
protected:
	InferenceConditions conditions;
	InferenceActions actions;
};

#endif /* _INFERENCERULE_H */
