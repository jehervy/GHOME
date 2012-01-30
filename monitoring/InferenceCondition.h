/*
 * InferenceCondition.h
 *
 *  Created on: 16 janv. 2012
 *      Author: vincent
 */

#ifndef _INFERENCECONDITION_H
#define _INFERENCECONDITION_H

#include <iostream>

class InferenceCondition {
public:
	static const int COND_SUP = 1;
	static const int COND_SUPEQ = 2;
	static const int COND_INF = 3;
	static const int COND_INFEQ = 4;
	static const int COND_EQ = 5;

	InferenceCondition(int metric, int comparator, int threshold);
	InferenceCondition(const InferenceCondition& condition);
	virtual ~InferenceCondition();
	bool match(int metric, int value);
private:
	int metric;
	int comparator;
	int threshold;
};

#endif /* _INFERENCECONDITION_H */
