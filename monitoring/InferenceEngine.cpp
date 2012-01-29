/*
 * InferenceEngine.cpp
 *
 *  Created on: 16 janv. 2012
 *      Author: vincent
 */

#include "InferenceEngine.h"
#include <stdlib.h>

InferenceEngine::InferenceEngine(char* file)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(file);
	pugi::xml_node xmlRules = doc.child("rules");
	std::cout << "Load result: " << result.description() << std::endl;

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

	for (std::vector<InferenceRule>::iterator rulesIt = rules.begin(); rulesIt != rules.end(); ++rulesIt)
	{
		if (rulesIt->match(metric, value))
		{
			for (InferenceActions::iterator actionsIt = rulesIt->getActions().begin(); actionsIt != rulesIt->getActions().end(); ++actionsIt)
			{
				result.push_back(*actionsIt);
			}
		}
	}

	return result;
}
