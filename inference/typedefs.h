#ifndef _INFERENCE_TYPEDEFS_H
#define _INFERENCE_TYPEDEFS_H

#include <vector>
#include <map>
#include <string>

#include "Condition.h"
#include "Rule.h"
#include "Action.h"

namespace inference
{

class Rule;

typedef std::vector<Condition> Conditions;
typedef std::vector<Action> Actions;
typedef std::vector<Rule> Rules;
typedef std::map <int, int> State;

}
#endif

