//Personal includes
#include "Test.h"

/**
 * Trivial.
 */
Test::Test() : testsLaunched(0), testsFailed(0)
{
}

/**
 * Checks the condition and prints the result in the console.
 */
void Test::assert(int condition, std::string message)
{
	testsLaunched++;

	if (!condition)
	{
		testsFailed++;
		std::cout << "\033[31m" << "#" << testsLaunched << " failed: " << message << "\033[30m" << std::endl;
	}
	else
	{
		std::cout << "#" << testsLaunched << " passed: " << message << std::endl;
	}
}

/**
 * Trivial.
 */
void Test::fail(std::string message)
{
	assert(0, message);
}

/**
 * Trivial.
 */
void Test::success(std::string message)
{
	assert(1, message);
}

/**
 * Records the tests as launched (and not failed) and prints they have
 * been skipped.
 */
void Test::skip(int number)
{
	for (int i = 0; i < number; i++)
	{
		testsLaunched++;
		std::cout << "#" << testsLaunched << " skipped" << std::endl;
	}
}

/**
 * Registers the callback and the prefix.
 */
void Test::add(TestFunction callback, std::string prefix)
{
	tests.push_back(callback);
	prefixes.push_back(prefix);
}

/**
 * Runs each test inside a section. Prints the global result.
 */
int Test::run()
{
	for (unsigned int i = 0; i < tests.size(); i++)
	{
		std::cout << "\033[33m" << "[" << prefixes[i] << "]" << "\033[30m" << std::endl;
		(*tests[i])(this);
		std::cout << std::endl;
	}

	if (testsFailed == 0)
	{
		std::cout << "\033[37;42m" << "All tests passed: " << testsLaunched << " tests OK" << std::endl;
	}
	else
	{
		std::cout << "\033[37;41m" << 100*(testsLaunched - testsFailed)/testsLaunched << " % sucessful: ";
		std::cout << testsFailed << " tests failed (" << testsLaunched << " total)" << std::endl;
	}

	int retval = testsFailed;

	testsLaunched = 0;
	testsFailed = 0;

	return retval;
}
