#ifndef _INFERENCE_CONDITION_H
#define _INFERENCE_CONDITION_H

namespace inference
{

class Condition
{
public:
	static const int COND_SUP = 1;
	static const int COND_SUPEQ = 2;
	static const int COND_INF = 3;
	static const int COND_INFEQ = 4;
	static const int COND_EQ = 5;

	/**
	 * Constructor.
	 *
	 * @param metric The metric to be tested
	 * @param comparator The comparator to use
	 * @param threshold The threshold to be compared with
	 */
	Condition(int metric, int comparator, int threshold);

	/**
	 * Copy constructor.
	 *
	 * @param condition the Condition instance to copy
	 */
	Condition(const Condition& condition);

	/**
	 * Checks if a given metric and value couple matches against
	 * this condition.
	 *
	 * @param metric The metric to test
	 * @param value The value to test
	 */
	bool match(int metric, int value);

private:
	int metric;
	int comparator;
	int threshold;
};

}
#endif /* _INFERENCE_CONDITION_H */
