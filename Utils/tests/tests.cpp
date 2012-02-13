/*
 * InferenceTests.cpp
 *
 *  Created on: 13 fev.. 2012
 *      Author: Pierre
 */

#include "tests.h"

#include "../SystemLog.h"


void TestSystemLog(Test *test)
{

	test->assert(SystemLog::AddLog(SystemLog::ERROR, "test erreur") == 0, "Insertion d'une erreur dans le log");
	test->assert(SystemLog::AddLog(SystemLog::SUCCESS, "test success") == 0, "Insertion d'un success dans le log");

//		test->skip(2);

}

void UtilsTestsMultipleActions(Test *test)
{
//	Actions actions;
//	Engine eng("inference/tests/multipleActions.xml");
//
//	actions = eng.run(1, 20);
//	test->assert(actions.size() == 2, "All actions are returned.");
//	if (actions.size() == 2)
//	{
//		test->assert(actions[0].getMetric() == 2 && actions[0].getValue() == 1, "Action 1 is ok.");
//		test->assert(actions[1].getMetric() == 3 && actions[1].getValue() == 42, "Action 2 is ok.");
//	}
}

void UtilsTestsMultipleConditions(Test *test)
{
//	Actions actions;
//	Engine eng("inference/tests/multipleConditions.xml");
//
//	actions = eng.run(1, 20);
//	test->assert(actions.size() == 0, "With one matching metric, rule does not match.");
//
//	actions = eng.run(3, 50);
//	test->assert(actions.size() == 1, "With the second matching metric, rule matches.");
}


void UtilsTests(Test *test)
{
	test->add(&TestSystemLog, "Utils");
	//test->add(&InferenceTestsMultipleActions, "InferenceMultipleActions");
	//test->add(&InferenceTestsMultipleConditions, "InferenceMultipleConditions");
}
