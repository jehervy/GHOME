#ifndef _INFERENCE_TYPES_H
#define _INFERENCE_TYPES_H

//System includes
#include <vector>
#include <map>

//Personal includes
#include "Condition.h"
#include "Action.h"

namespace inference
{

typedef std::vector<Condition> Conditions;
typedef std::vector<Action> Actions;
typedef std::map <int, int> State;

}

#endif /* _INFERENCE_TYPES_H */



