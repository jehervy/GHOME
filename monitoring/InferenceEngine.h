/*
 * InferenceEngine.h
 *
 *  Created on: 16 janv. 2012
 *      Author: vincent
 */

#ifndef _INFERENCEENGINE_H
#define _INFERENCEENGINE_H

#include <iostream>
#include <vector>

#include "InferenceRule.h"
#include "InferenceAction.h"
#include "../xml/pugixml.hpp"

typedef std::vector<InferenceAction> InferenceActions;
typedef std::vector<InferenceRule> InferenceRules;

class InferenceEngine {
public:
	/**
	 * Builds the inference engine in loading all rules defined in a XML
	 * configuration file.
	 *
	 * @param file Path to the XML configuration file
	 */
	InferenceEngine(char* file);

	/**
	 * Destructor.
	 */
	virtual ~InferenceEngine();

	/**
	 * Runs the engine against the given metric with the given value.
	 *
	 * @param metric Identifier of the metric
	 * @param value Value of the metric
	 */
	InferenceActions run(int metric, int value);

private:
	InferenceRules rules;
};

#endif /* _INFERENCEENGINE_H */
