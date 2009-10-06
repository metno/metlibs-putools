/*
 * Test cases for the miString class
 *
 * $Id$
 */

#include <miString>

#include <check/tap>

using namespace miutil;
using namespace tap;

// doesn't really do anything interesting
class test_new_miString_const_char: public TestCase {
	TestOutcome run() {
		miString str("Hello, world!");
		why = str;
		return pass;
	}
};

class test_new_miString_std_string: public TestCase {
	TestOutcome run() {
		miString str(std::string("Hello, world!"));
		why = str;
		return pass;
	}
};

class test_new_miString_contains: public TestCase {
	TestOutcome run() {
		miString str("supercalifragilisticexpialadocious");
		const char *sub = "fragilistic";
		bool contains = str.contains(sub);
		why = "miString(\"" + str + "\")." +
		    "contains(\"" + sub + "\") = " +
		    (contains ? "true" : "false");
		return contains ? pass : fail;
	}
};

class test_new_miString_contains_no: public TestCase {
	TestOutcome run() {
		miString str("supercalifragilisticexpialadocious");
		const char *sub = "nihilistic";
		bool contains = str.contains(sub);
		why = "miString(\"" + str + "\")." +
		    "contains(\"" + sub + "\") = " +
		    (contains ? "true" : "false");
		return contains ? fail : pass;
	}
};

int
main()
{
	TestPlan tp;
	tp.comment("miString");
	tp.add(new test_new_miString_const_char());
	tp.add(new test_new_miString_std_string());
	tp.add(new test_new_miString_contains());
	tp.add(new test_new_miString_contains_no());
	tp.run();
	return (tp.am_exit_code());
}
