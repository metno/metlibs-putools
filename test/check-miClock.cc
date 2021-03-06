/*
  libpuTools - Basic types/algorithms/containers

  Copyright (C) 2006-2019 met.no

  Contact information:
  Norwegian Meteorological Institute
  Box 43 Blindern
  0313 OSLO
  NORWAY
  email: diana@met.no

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

// Test cases for the miClock class

#include "miTime.h"
#include <gtest/gtest.h>

using miutil::miClock;
using miutil::miDate;
using miutil::miTime;

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

TEST(MiTimeTest, ctor)
{
    {
        miTime t(2013, 1, 1, 22, 58, 58);
        EXPECT_EQ("2013-01-01 22:58:58", t.isoTime());
    }
    {
        miTime t(miDate(2013, 1, 1), miClock(22, 58, 59));
        EXPECT_EQ("2013-01-01 22:58:59", t.isoTime());
    }
    {
        miTime t("2013-01-01 22:58:59");
        EXPECT_EQ("2013-01-01 22:58:59", t.isoTime());
    }

    {
        miTime t;
        EXPECT_TRUE(t.undef());
        t.setTime("2013-01-01 22:58:59");
        EXPECT_EQ("2013-01-01 22:58:59", t.isoTime());
    }
}

TEST(MiTimeTest, format)
{
    {
        const miTime t(2013, 1, 1, 22, 58, 58);
        EXPECT_EQ("2013-01-01 22:58:58", t.format("$time"));
    }
}

TEST(MiTimeTest, isoTime)
{
  const miTime t(2013, 1, 1, 22, 58, 58);
  EXPECT_EQ("2013-01-01 22:58:58", t.isoTime());
  EXPECT_EQ("2013-01-01T22:58:58", t.isoTime("T"));

  EXPECT_EQ("2013-01-01 22:58:58", t.isoTime(true, true));
  EXPECT_EQ("2013-01-01 22:58", t.isoTime(true, false));
  EXPECT_EQ("2013-01-01 22", t.isoTime(false, true));
  EXPECT_EQ("2013-01-01 22", t.isoTime(false, false));
}

TEST(MiTimeTest, formatStatic)
{
  {
    const miTime t(2013, 1, 1, 22, 58, 58);
    EXPECT_EQ("2013-01-01 22:58:58", miTime::format(t, "%Y-%m-%d %H:%M:%S"));
  }
  {
    // swap first two digits of year -- this moves the year outside the
    // accepted timespan for boost::posix_time, which yields an exception,
    // which we do not want to see
    const miTime t("0213-01-01 22:58:58");
    ASSERT_NO_THROW(miTime::format(t, "%Y-%m-%d %H:%M:%S"));
  }
}

TEST(MiDateTest, format)
{
    {
        const miDate d(2013, 1, 1);
        EXPECT_EQ("2013-01-01", d.format("%Y-%m-%d"));
    }
}

TEST(MiDateTest, formatWeekNumber)
{
    {
        const miDate d(2018, 11, 11);
        EXPECT_EQ("45", d.format("%V"));
    }
}

TEST(MiTimeTest, FromText)
{
  const miTime t(2013, 1, 1, 22, 58, 58);
  EXPECT_EQ(miTime("2013-01-01 22:58:58"), t);

  EXPECT_EQ(miTime("20130101T225858"), t);
}
