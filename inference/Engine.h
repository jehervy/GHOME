#ifndef _INFERENCE_ENGINE_H
#define _INFERENCE_ENGINE_H

//System includes
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cstdlib>

//Personal includes
#include "types.h"
#include "Rule.h"
#include "../xml/pugixml.hpp"

namespace inference
{

typedef std::vector<Rule> Rules;

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
	 * Runs the engine against the given metric and value couple.
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

