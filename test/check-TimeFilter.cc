
#include <TimeFilter.h>

#include <gtest/gtest.h>

static std::string tf_replaced(std::string pattern)
{
  miutil::TimeFilter tf(pattern);
  return pattern;
}

TEST(TimeFilterTest, PatternAdvanced)
{
  EXPECT_EQ("C11/C11_????????????+?H?M", tf_replaced("C11/C11_[yyyymmddHHMM]+?H?M"));
  EXPECT_EQ("A_????????HHMM??_??", tf_replaced("A_[yyyymmdd]HHMM[HH]_[MM]"));
  EXPECT_EQ("A_????????HH????_MM", tf_replaced("A_[yyyymmdd]HH[MMHH]_MM"));

  // ignore unknown pattern chars
  EXPECT_EQ("A_????_????????", tf_replaced("A_[yyyy_mmdd][MMHH]"));
}

// ------------------------------------------------------------------------

static std::string tf_timestring(std::string pattern, std::string filename)
{
  miutil::TimeFilter tf(pattern);
  return tf.getTimeStr(filename);
}

TEST(TimeFilterTest, Time)
{
  EXPECT_EQ("2001-02-03T04:05:00", tf_timestring("C11_[yyyymmddHHMM]+?H?M", "C11_200102030405+xHyM"));
  EXPECT_EQ("2011-01-02T03:04:00", tf_timestring("A_[yyyymmdd]HHMM[HH]_[MM]", "A_20110102HHMM03_04"));

  EXPECT_EQ("2011-01-02T03:04:00", tf_timestring("A[yyyy]/[mm]/[dd]/x_[HH][MM]", "A2011/01/02/x_0304"));
  EXPECT_EQ("2015-02-01T18:00:00", tf_timestring("arome_[yyyymmdd]_[HH]_vc.nc", "test/arome_20150201_18_vc.nc"));

}
