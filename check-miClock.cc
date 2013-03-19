/*
 * Test cases for the miClock class
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define METLIBS_SUPPRESS_DEPRECATED
#include "miClock.h"
#include "miString.h"
#include <gtest/gtest.h>

using miutil::miClock;

TEST(MiClockTest, ctor)
{
    {   const miClock c;
        EXPECT_TRUE(c.hour() == -1 && c.min() == -1 && c.sec() == -1);
    }
    {   int h = 1, m = 2, s = 3;
        const miClock c(h, m, s);
        EXPECT_TRUE(c.hour() == h && c.min() == m && c.sec() == s);
    }
    {   const std::string str("01:02:03");
        const miClock c(str);
        EXPECT_TRUE(c.hour() == 1 && c.min() == 2 && c.sec() == 3);
    }
};

TEST(MiClockTest, isoClock)
{
    const int h = 1, m = 2, s = 3;
    const miClock c(h, m, s);
    const std::string iso = c.isoClock();
    EXPECT_TRUE(iso == "01:02:03");
}

TEST(MiClockTest, add)
{
    { miClock add(22, 58, 58); add.addHour(0); add.addMin(0); add.addSec(1); EXPECT_EQ("22:58:59", add.isoClock()); }
    { miClock add(22, 58, 59); add.addHour(0); add.addMin(0); add.addSec(1); EXPECT_EQ("22:59:00", add.isoClock()); }
    { miClock add(22, 59, 59); add.addHour(0); add.addMin(0); add.addSec(1); EXPECT_EQ("23:00:00", add.isoClock()); }
    { miClock add(23, 59, 59); add.addHour(0); add.addMin(0); add.addSec(1); EXPECT_EQ("00:00:00", add.isoClock()); }
    { miClock add(22, 58,  7); add.addHour(0); add.addMin(1); add.addSec(0); EXPECT_EQ("22:59:07", add.isoClock()); }
    { miClock add(22, 59,  7); add.addHour(0); add.addMin(1); add.addSec(0); EXPECT_EQ("23:00:07", add.isoClock()); }
    { miClock add(23, 59,  7); add.addHour(0); add.addMin(1); add.addSec(0); EXPECT_EQ("00:00:07", add.isoClock()); }
    { miClock add(22,  7,  7); add.addHour(1); add.addMin(0); add.addSec(0); EXPECT_EQ("23:07:07", add.isoClock()); }
    { miClock add(23,  7,  7); add.addHour(1); add.addMin(0); add.addSec(0); EXPECT_EQ("00:07:07", add.isoClock()); }
}
