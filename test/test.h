#ifndef _TEST_H
#define _TEST_H

//System includes.
#include <iostream>
#include <string>
#include <vector>

class Test;

typedef void (*TestFunction) (Test*);
typedef std::vector<TestFunction> Tests;
typedef std::vector<std::string> Prefixes;

class Test
{
public:
	/**
	 * Default constructor.
	 */
	Test();

	/**
	 * Asserts that something is correct.
	 *
	 * @param condition The thing assert to be true
	 * @param std::string The thing tested (human-readable)
	 */
	void assert(int condition, std::string message);

	/**
	 * Skips a defined amount of tests.
	 *
	 * @param number Number of tests to skip
	 */
	void skip(int number = 1);

	/**
	 * Manually marks a test as failed.
	 *
	 * @param std::string The thing tested (and failed)
	 */
	void fail(std::string message);

	/**
	 * Manually marks a test as success.
	 *
	 * @param std::string The thing tested (and succeed)
	 */
	void success(std::string message);

	/**
	 * Registers a test to execute.
	 *
	 * @param callback The test function (which will receive the test object as first argument)
	 * @param std::string The prefix to group tests
	 */
	void add(TestFunction callback, std::string prefix = "Tests");

	/**
	 * Runs the registered tests.
	 *
	 * @return integer Number of tests failed
	 */
	int run();

private:
	int testsLaunched;
	int testsFailed;
	Tests tests;
	Prefixes prefixes;
};

#endif

