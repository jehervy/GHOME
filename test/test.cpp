#include "test.h"

using namespace std;

Test::Test() : testsLaunched(0), testsFailed(0)
{
}

void Test::assert(int condition, string message)
{
	testsLaunched++;

	if (!condition)
	{
		testsFailed++;
		cout << "#" << testsLaunched << " failed: " << message << endl;
	}
	else
	{
		cout << "#" << testsLaunched << " passed: " << message << endl;
	}
}

void Test::skip(int number)
{
	for (int i = 0; i < number; i++)
	{
		testsLaunched++;
		cout << "#" << testsLaunched << " skipped" << endl;
	}
}

void Test::add(TestFunction callback, string prefix)
{
	tests.push_back(callback);
	prefixes.push_back(prefix);
}

int Test::run()
{
	for (Tests::iterator it = tests.begin(); it != tests.end(); ++it)
	{
		(**it)(this);
	}

	cout << endl;
	if (testsFailed == 0)
	{
		cout << "All tests passed: " << testsLaunched << " tests OK." << endl;
	}
	else
	{
		cout << 100*(testsLaunched - testsFailed)/testsLaunched << " % sucessful: ";
		cout << testsFailed << " tests failed (" << testsLaunched << " total)." << endl;
	}

	int retval = testsFailed;

	testsLaunched = 0;
	testsFailed = 0;

	return retval;
}
