
#include <TimeFilter.h>

#include <gtest/gtest.h>

static std::string tf_replaced(std::string pattern, bool advanced)
{
  miutil::TimeFilter tf(pattern, advanced);
  return pattern;
}

TEST(TimeFilterTest, PatternAdvanced)
{
  EXPECT_EQ("C11/C11_????????????+?H?M", tf_replaced("C11/C11_[yyyymmddHHMM]+?H?M", true));
  EXPECT_EQ("A_????????HHMM??_??", tf_replaced("A_[yyyymmdd]HHMM[HH]_[MM]", true));
  EXPECT_EQ("A_????????HH????_MM", tf_replaced("A_[yyyymmdd]HH[MMHH]_MM", true));

  // ignore unknown pattern chars
  EXPECT_EQ("A_????_????????", tf_replaced("A_[yyyy_mmdd][MMHH]", true));
}

TEST(TimeFilterTest, PatternSimple)
{
  EXPECT_EQ("X_????????????+???HH??MM", tf_replaced("X_yyyymmddHHMM+???HH??MM", false));
}

// ------------------------------------------------------------------------

static std::string tf_timestring(std::string pattern, bool advanced, std::string filename)
{
  miutil::TimeFilter tf(pattern, advanced);
  return tf.getTimeStr(filename);
}

TEST(TimeFilterTest, Time)
{
  EXPECT_EQ("2001-02-03T04:05:00", tf_timestring("C11_[yyyymmddHHMM]+?H?M", true, "C11_200102030405+xHyM"));
  EXPECT_EQ("2011-01-02T03:04:00", tf_timestring("A_[yyyymmdd]HHMM[HH]_[MM]", true, "A_20110102HHMM03_04"));

  EXPECT_EQ("2011-01-02T03:04:00", tf_timestring("A[yyyy]/[mm]/[dd]/x_[HH][MM]", true, "A2011/01/02/x_0304"));
  EXPECT_EQ("2015-02-01T18:00:00", tf_timestring("arome_[yyyymmdd]_[HH]_vc.nc", true, "test/arome_20150201_18_vc.nc"));

  EXPECT_EQ("2011-01-02T03:04:00", tf_timestring("X_yyyymmddHHMM+???HH??MM", false, "X_201101020304+???HH??MM"));

}
