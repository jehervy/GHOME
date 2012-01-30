/*
 * InferenceEngine.cpp
 *
 *  Created on: 16 janv. 2012
 *      Author: vincent
 */

#include "InferenceEngine.h"
#include <stdlib.h>

using namespace std;

InferenceEngine::InferenceEngine(char* file)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(file);
	pugi::xml_node xmlRules = doc.child("rules");

	if (!((bool) result))
	{
		std::cout << "Error while loading inference engine rules: " << result.description() << std::endl;
	}

	//Loop over the rules defined in the XML file.
	for (pugi::xml_node_iterator rulesIt = xmlRules.begin(); rulesIt != xmlRules.end(); ++rulesIt)
	{
		pugi::xml_node xmlConditions = rulesIt->child("if");
		pugi::xml_node xmlActions = rulesIt->child("then");
		InferenceRule rule;

		//Adds all conditions defined in the XML file.
		for (pugi::xml_node_iterator conditionsIt = xmlConditions.begin(); conditionsIt != xmlConditions.end(); ++conditionsIt)
		{
			rule.addCondition(conditionsIt->attribute("id").as_int(), (char *) conditionsIt->attribute("cond").value(), atoi(conditionsIt->child_value()));
			//Note: there seems to be no way to cast properly the child_value(), so we use atoi...
		}

		//Adds all actions defined in the XML file.
		for (pugi::xml_node_iterator actionsIt = xmlActions.begin(); actionsIt != xmlActions.end(); ++actionsIt)
		{
			rule.addAction(actionsIt->attribute("id").as_int(), atoi(actionsIt->child_value()));
		}

		rules.push_back(rule);
	}
}

/**
 * Do nothing.
 */
InferenceEngine::~InferenceEngine()
{
}

InferenceActions InferenceEngine::run(int metric, int value)
{
	InferenceActions result;

	//Updates the current state for the given metric.
	currentState[metric] = value;

	for (InferenceRules::iterator rulesIt = rules.begin(); rulesIt != rules.end(); ++rulesIt)
	{
		for (State::iterator stateIt = currentState.begin(); stateIt != currentState.end(); stateIt++)
		{
			if (rulesIt->match(stateIt->first, stateIt->second))
			{
				for (unsigned int i = 0; i < rulesIt->getActions().size(); i++)
				{
					result.push_back(rulesIt->getActions()[i]);
				}
			}
		}
	}

	return result;
}
