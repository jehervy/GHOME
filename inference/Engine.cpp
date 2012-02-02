#include "Engine.h"

/**
 * Parses the XML file given as parameter and transform it in a set
 * of rules.
 */
inference::Engine::Engine(std::string file)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(file.c_str());
	pugi::xml_node xmlRules = doc.child("rules");

	//The xml_parse_result object is castable into boolean to determine if
	//there was a failure during parsing.
	if (!((bool) result))
	{
		std::cerr << "Error while loading inference engine rules: " << result.description() << std::endl;
		return;
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
			rule.addCondition(conditionsIt->attribute("id").as_int(), conditionsIt->attribute("cond").value(), atoi(conditionsIt->child_value()));
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
 * The engine holds internally the current state. It means that previous
 * values of other metrics are remembered and don't need to be passed
 * every time. Only the given metric and value couple erases the old state.
 *
 * We then only have to loop over all rules, check if it match and if yes
 * merge the actions to be done with the result set.
 */
inference::Actions inference::Engine::run(int metric, int value)
{
	inference::Actions result;

	//Updates the current state for the given metric.
	currentState[metric] = value;

	//Loop over the rules set.
	for (Rules::iterator rulesIt = rules.begin(); rulesIt != rules.end(); ++rulesIt)
	{
		if (rulesIt->match(currentState))
		{
			//Here is the tricky part: the rule match and we have to merge
			//the actions to be done in a consistent way in consideration
			//with results given by other rules.
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

				//If the action as not been found, we can add it safely.
				if (!found)
				{
					result.push_back(action);
				}
			}
		}
	}

	return result;
}

