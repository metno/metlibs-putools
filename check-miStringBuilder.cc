/*
 * Test cases for the miutil::StringBuilder class
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define METLIBS_SUPPRESS_DEPRECATED
#include "miStringBuilder.h"
#include <gtest/gtest.h>

TEST(miStringBuilderTest, simple)
{
  const std::string message = (miutil::StringBuilder() << "five plus " << 2 << " is seven");
  ASSERT_EQ("five plus 2 is seven", message);
}
