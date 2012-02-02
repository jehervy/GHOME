/*
 * InferenceAction.h
 *
 *  Created on: 16 janv. 2012
 *      Author: vincent
 */

#ifndef _INFERENCE_ACTION_H
#define _INFERENCE_ACTION_H

namespace inference
{

class Action
{
public:
	Action(const Action& action);
	Action(int metric, int value);
	virtual ~Action();

	bool equals(const Action& action);
	void merge(const Action& action);
	int getMetric();
	int getValue();
private:
	int metric;
	int value;
};

}
#endif /* _INFERENCE_ACTION_H */
