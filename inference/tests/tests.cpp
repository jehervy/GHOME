#include "tests.h"

using namespace inference;

void InferenceTestsSimple(Test *test)
{
	Actions actions;
	Engine eng("src/inference/tests/simple.xml");

	actions = eng.run(2, 20);
	test->assert(actions.size() == 0, "Metric to match is ok.");

	actions = eng.run(1, 15);
	test->assert(actions.size() == 0, "Value to match is ok.");

	actions = eng.run(1, 20);
	test->assert(actions.size() == 1, "Matching condition is taken in account.");
	if (actions.size() == 1)
	{
		test->assert(actions[0].getMetric() == 2, "Metric to apply is ok.");
		test->assert(actions[0].getValue() == 1, "Value to apply is ok.");
	}
	else
	{
		test->skip(2);
	}
}

void InferenceTestsMultipleActions(Test *test)
{
	Actions actions;
	Engine eng("src/inference/tests/multipleActions.xml");

	actions = eng.run(1, 20);
	test->assert(actions.size() == 2, "All actions are returned.");
	if (actions.size() == 2)
	{
		test->assert(
			actions[0].getMetric() == 2 && actions[0].getValue() == 1 &&
			actions[1].getMetric() == 3 && actions[1].getValue() == 42,
			"All actions are correct."
		);
	}
}

void InferenceTestsMultipleConditions(Test *test)
{
	Actions actions;
	Engine eng("src/inference/tests/multipleConditions.xml");

	actions = eng.run(1, 20);
	test->assert(actions.size() == 0, "With one matching metric, rule does not match.");

	actions = eng.run(3, 50);
	test->assert(actions.size() == 1, "With the second matching metric, rule matches.");
}

void InferenceTestsMultipleRules(Test *test)
{
	Actions actions;
	Engine eng("src/inference/tests/multipleRules.xml");

	actions = eng.run(1, 23);
	test->assert(
		actions.size() == 1 && actions[0].getMetric() == 2 && actions[0].getValue() == 1,
		"Only the correct rules match."
	);

	actions = eng.run(1, 19);
	test->assert(
		actions.size() == 1 && actions[0].getMetric() == 2 && actions[0].getValue() == 4,
		"With multiple correct rules, result is merged (with max strategy)."
	);
}

void InferenceTests(Test *test)
{
	test->add(&InferenceTestsSimple, "Inference");
	test->add(&InferenceTestsMultipleActions, "Inference");
	test->add(&InferenceTestsMultipleConditions, "Inference");
	test->add(&InferenceTestsMultipleRules, "Inference");
}
