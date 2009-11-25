/*
 * Test cases for the miClock class
 *
 * $Id$
 */

#include <boost/format.hpp>
#include <miClock.h>
#include <miString.h>

#include <check/tap>

using namespace miutil;
using namespace tap;

class test_new_miClock_default: public TestCase {
	TestOutcome run(void) {
		miClock c;
		why = (boost::format("miClock() = (%1%, %2%, %3%)") %
		    c.hour() % c.min() % c.sec()).str();
		if (c.hour() == -1 && c.min() == -1 && c.sec() == -1)
			return pass;
		return fail;
	}
};

class test_new_miClock_h_m_s: public TestCase {
	TestOutcome run(void) {
		int h = 1, m = 2, s = 3;
		miClock c(h, m, s);
		why = (boost::format("miClock(%1%, %2%, %3%) = (%4%, %5%, %6%)") %
		    h % m % s % c.hour() % c.min() % c.sec()).str();
		if (c.hour() == h && c.min() == m && c.sec() == s)
			return pass;
		return fail;
	}
};

class test_new_miClock_miString: public TestCase {
	TestOutcome run(void) {
		miString str("01:02:03");
		miClock c(str);
		why = (boost::format("miClock(\"%1%\") = (%2%, %3%, %4%)") %
		    str % c.hour() % c.min() % c.sec()).str();
		if (c.hour() == 1 && c.min() == 2 && c.sec() == 3)
			return pass;
		return fail;
	}
};

class test_miClock_isoClock: public TestCase {
	TestOutcome run(void) {
		int h = 1, m = 2, s = 3;
		miClock c(h, m, s);
		miString iso = c.isoClock();
		why = (boost::format("miClock(%1%, %2%, %3%) = \"%4%\"") %
		    h % m % s % iso).str();
		if (iso == "01:02:03")
			return pass;
		return fail;
	}
};

/*
 * Test the addSec(), addMin() and addHour() methods.  There are many
 * different edge cases we want to test, so instead of writing a different
 * class for each, we write a single flexible class that can test them
 * all.
 */
class test_miClock_add: public TestCase {
	TestOutcome run(void) {
		miClock c(h, m, s);
		why = (boost::format("miClock(%1%, %2%, %3%)") % h % m % s).str();
		if (as) {
			c.addSec(as);
			why += (boost::format(".addSec(%1%)") % as).str();
			s += as;
		}
		if (am) {
			c.addMin(am);
			why += (boost::format(".addMin(%1%)") % am).str();
			m += am;
		}
		if (ah) {
			c.addHour(ah);
			why += (boost::format(".addHour(%1%)") % ah).str();
			h += ah;
		}
		why += (boost::format(" = (%1%, %2%, %3%)") %
		    c.hour() % c.min() % c.sec()).str();
		m += s / 60;
		s %= 60;
		h += m / 60;
		m %= 60;
		h %= 24;
		if (c.hour() == h && c.min() == m && c.sec() == s)
			return pass;
		return fail;
	}

public:
	test_miClock_add(int h, int m, int s, int ah, int am, int as):
		h(h), m(m), s(s), ah(ah), am(am), as(as) {};

private:
	int h, m, s;
	int ah, am, as;
};

int
main(void)
{
	TestPlan tp;
	tp.comment("miClock");
	tp.add(new test_new_miClock_default());
	tp.add(new test_new_miClock_h_m_s());
	tp.add(new test_new_miClock_miString());
	tp.add(new test_miClock_isoClock());
	tp.add(new test_miClock_add(22, 58, 58,   0, 0, 1));
	tp.add(new test_miClock_add(22, 58, 59,   0, 0, 1));
	tp.add(new test_miClock_add(22, 59, 59,   0, 0, 1));
	tp.add(new test_miClock_add(23, 59, 59,   0, 0, 1));
	tp.add(new test_miClock_add(22, 58,  7,   0, 1, 0));
	tp.add(new test_miClock_add(22, 59,  7,   0, 1, 0));
	tp.add(new test_miClock_add(23, 59,  7,   0, 1, 0));
	tp.add(new test_miClock_add(22,  7,  7,   1, 0, 0));
	tp.add(new test_miClock_add(23,  7,  7,   1, 0, 0));
	tp.run();
	return (tp.am_exit_code());
}
