/*
 * Test cases for the miString class
 *
 * $Id$
 */

#include <iostream>
#include <miString>

#include <check/tap>

using namespace miutil;
using namespace tap;

// doesn't really do anything interesting
class test_new_miString_const_char: public TestCase {
	TestOutcome run(void) {
		miString str("Hello, world!");
		why = str;
		return pass;
	}
};

class test_new_miString_std_string: public TestCase {
	TestOutcome run(void) {
		miString str(std::string("Hello, world!"));
		why = str;
		return pass;
	}
};

class test_new_miString_contains: public TestCase {
	TestOutcome run(void) {
		miString str("supercalifragilisticexpialadocious");
		return str.contains("fragil") ? pass : fail;
	}
};

int
main(void)
{
	TestSet ts;
	ts.comment("miString");
	ts.add(new test_new_miString_const_char());
	ts.add(new test_new_miString_std_string());
	ts.add(new test_new_miString_contains());
	ts.run();
	return (ts.am_exit_code());
}
