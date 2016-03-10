
#include "puMathAlgo.h"

#include <gtest/gtest.h>

#include <set>
#include <vector>

using namespace puMathAlgo;

TEST(MathAlgoTest, Percentile)
{
  const int values[10] = { 1, 1, 3,  4, 4, 5,  6, 8, 8, 9 };
  const std::vector<int> some(values, values + 10);

  EXPECT_EQ(3, percentile(some, 0.3));
  EXPECT_EQ(8, percentile(some, 0.8));
}

TEST(MathAlgoTest, Mean)
{
  const float values[10] = { 1.0, 1.1, 3.0,  4.1, 4.6, 5.2,  6.6, 8.7, 8.9, 9.1 };
  const std::vector<float> some(values, values + 10);

  ASSERT_FLOAT_EQ(5.23, mean(some));
}

TEST(MathAlgoTest, AverageFloat)
{
  const float values[10] = { 1.0, 1.1, 3.0,  4.1, 4.6, 5.2,  6.6, 8.7, 8.9, 9.1 };

  average<float> avg1(0), avg2(0);
  for (int i=0; i<5; ++i) {
    avg1.add(values[i  ]);
    avg2.add(values[i+5]);
  }

  EXPECT_FLOAT_EQ(2.76, avg1());
  EXPECT_FLOAT_EQ(7.70, avg2());

  average<float> avg = avg1 + avg2;
  EXPECT_FLOAT_EQ(5.23, avg());
}
