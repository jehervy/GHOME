/*
 * InferenceEngine.h
 *
 *  Created on: 16 janv. 2012
 *      Author: vincent
 */

#ifndef _INFERENCE_ENGINE_H
#define _INFERENCE_ENGINE_H

#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "typedefs.h"
#include "../xml/pugixml.hpp"

namespace inference
{

class Engine
{
public:
	/**
	 * Builds the inference engine in loading all rules defined in a XML
	 * configuration file.
	 *
	 * @param file Path to the XML configuration file
	 */
	Engine(std::string file);

	/**
	 * Runs the engine against the given metric with the given value.
	 *
	 * @param metric Identifier of the metric
	 * @param value Value of the metric
	 */
	Actions run(int metric, int value);

private:
	Rules rules;
	State currentState;
};

}
#endif /* _INFERENCE_ENGINE_H */

