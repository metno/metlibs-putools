
#include "minmax.h"

#include <gtest/gtest.h>

#include <set>
#include <vector>

TEST(MinMaxTest, Vector)
{
  std::vector<int> some;
  some.push_back(2);
  some.push_back(1);
  some.push_back(3);

  int mini = 0, maxi = 0;
  ASSERT_TRUE(MinMax(some, mini, maxi));
  ASSERT_EQ(1, mini);
  ASSERT_EQ(3, maxi);

  ASSERT_TRUE(MinMax(some, mini, maxi, std::greater<int>()));
  ASSERT_EQ(3, mini);
  ASSERT_EQ(1, maxi);
}

TEST(MinMaxTest, Set)
{
  std::set<int> some;
  some.insert(2);
  some.insert(1);
  some.insert(3);

  int mini = 0, maxi = 0;
  ASSERT_TRUE(MinMax(some, mini, maxi));
  ASSERT_EQ(1, mini);
  ASSERT_EQ(3, maxi);

  ASSERT_TRUE(MinMax(some, mini, maxi, std::greater<int>()));
  ASSERT_EQ(3, mini);
  ASSERT_EQ(1, maxi);
}

TEST(MinMaxTest, Array)
{
  const int some[3] = { 2, 1, 3 };

  int mini = 0, maxi = 0;
  ASSERT_TRUE(MinMax(some, some+3, mini, maxi));
  ASSERT_EQ(1, mini);
  ASSERT_EQ(3, maxi);

  ASSERT_TRUE(MinMax(some, some+3, mini, maxi, std::greater<int>()));
  ASSERT_EQ(3, mini);
  ASSERT_EQ(1, maxi);
}
