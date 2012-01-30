#ifndef _INFERENCE_TYPEDEFS_H
#define _INFERENCE_TYPEDEFS_H

#include <vector>
#include <map>
#include <string>

class InferenceCondition;
class InferenceAction;
class InferenceRule;

typedef std::vector<InferenceCondition> InferenceConditions;
typedef std::vector<InferenceAction> InferenceActions;
typedef std::vector<InferenceRule> InferenceRules;
typedef std::map <int, int> State;

#endif

