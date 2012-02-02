/*
 * InferenceEngine.cpp
 *
 *  Created on: 16 janv. 2012
 *      Author: vincent
 */

#include "Engine.h"
#include <stdlib.h>

using namespace std;

inference::Engine::Engine(string file)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(file.c_str());
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
		inference::Rule rule;

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

inference::Actions inference::Engine::run(int metric, int value)
{
	inference::Actions result;

	//Updates the current state for the given metric.
	currentState[metric] = value;

	for (Rules::iterator rulesIt = rules.begin(); rulesIt != rules.end(); ++rulesIt)
	{
		if (rulesIt->match(currentState))
		{
			for (unsigned int i = 0; i < rulesIt->getActions().size(); i++)
			{
				inference::Action action = rulesIt->getActions()[i];
				bool found = false;
				for (Actions::iterator resultIt = result.begin(); resultIt != result.end(); ++resultIt)
				{
					if (resultIt->equals(action))
					{
						resultIt->merge(action);
						found = true;
						break;
					}
				}

				if (!found)
				{
					result.push_back(action);
				}
			}
		}
	}

	return result;
}

