#ifndef _TEST_H
#define _TEST_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Test;

typedef void (*TestFunction) (Test*);
typedef vector<TestFunction> Tests;
typedef vector<string> Prefixes;

class Test
{
public:
	Test();
	void assert(int condition, string message);
	void add(TestFunction callback, string prefix = "");
	int run();
private:
	int testsLaunched;
	int testsFailed;
	Tests tests;
	Prefixes prefixes;
};

#endif

