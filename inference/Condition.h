/*
 * InferenceCondition.h
 *
 *  Created on: 16 janv. 2012
 *      Author: vincent
 */

#ifndef _INFERENCE_CONDITION_H
#define _INFERENCE_CONDITION_H

#include <iostream>

namespace inference
{

class Condition
{
public:
	static const int COND_SUP = 1;
	static const int COND_SUPEQ = 2;
	static const int COND_INF = 3;
	static const int COND_INFEQ = 4;
	static const int COND_EQ = 5;

	Condition(int metric, int comparator, int threshold);
	Condition(const Condition& condition);
	virtual ~Condition();
	bool match(int metric, int value);
private:
	int metric;
	int comparator;
	int threshold;
};

}
#endif /* _INFERENCE_CONDITION_H */
