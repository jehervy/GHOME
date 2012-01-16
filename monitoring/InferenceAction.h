/*
 * InferenceAction.h
 *
 *  Created on: 16 janv. 2012
 *      Author: vincent
 */

#ifndef _INFERENCEACTION_H
#define _INFERENCEACTION_H

class InferenceAction {
public:
	InferenceAction(int metric, int value);
	virtual ~InferenceAction();
	int getMetric();
	int getValue();
private:
	int metric;
	int value;
};

#endif /* _INFERENCEACTION_H */
