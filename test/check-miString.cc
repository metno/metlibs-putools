/*
 * Test cases for the miString class
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define METLIBS_SUPPRESS_DEPRECATED
#include "miString.h"
#include <gtest/gtest.h>
#include <boost/range.hpp>

using miutil::miString;

TEST(miStringTest, ctor)
{
    const miString s1("Hello, world!");
    ASSERT_EQ("Hello, world!", s1);

    const miString s2(std::string("Hello, world!"));
    ASSERT_EQ("Hello, world!", s2);
}

TEST(miStringTest, contains)
{
    const miString str("supercalifragilisticexpialadocious");
    const char *sub1 = "fragilistic";
    EXPECT_TRUE(str.contains(sub1));

    const char *sub2 = "nihilistic";
    EXPECT_FALSE(str.contains(sub2));

    EXPECT_TRUE(miutil::contains("this is my haystack", "hay"));
    EXPECT_FALSE(miutil::contains("this is my haystack", "needle"));
}

TEST(miStringTest, trim)
{
    const char t1[] = " hi  ";
    const char eb[] = "hi";
    const char er[] = " hi";
    const char el[] = "hi  ";

    miString ab(t1); ab.trim();
    EXPECT_EQ(eb, ab);
    miString ar(t1); ar.trim(false, true);
    EXPECT_EQ(er, ar);
    miString al(t1); al.trim(true, false);
    EXPECT_EQ(el, al);
    
    EXPECT_EQ(eb, miutil::trimmed(t1));
    EXPECT_EQ(er, miutil::trimmed(t1, false, true));
    EXPECT_EQ(el, miutil::trimmed(t1, true, false));

    const char t2[] = " hi  xx";
    const char e2[] = " hi  ";
    miString a2(t2); a2.trim(true, true, "x");
    EXPECT_EQ(e2, a2);
    EXPECT_EQ(e2, miutil::trimmed(t2, true, true, "x"));
}

TEST(miStringTest, trim_remove_empty)
{
    const char* t1[] = { " hi  ", " you", "out ", "", "there", "" };
    std::vector<std::string> vt1(t1, boost::end(t1));

    const char* e1[] = { "hi", "you", "out", "there" };
    const std::vector<std::string> ve1(e1, boost::end(e1));

    miutil::trim_remove_empty(vt1);
    ASSERT_EQ(ve1, vt1);
}

TEST(miStringTest, split)
{
    const char t1[] = " this   is a  string  ";
    const char* e1[] = { "this", "is", "a  string" };
    const char* e2[] = { "this", "is", "a", "string" };
    const std::vector<std::string> ve1(e1, boost::end(e1));
    const std::vector<std::string> ve2(e2, boost::end(e2));

    {
        const std::vector<miString> ms1 = miString(t1).split(2);
        const std::vector<miString> ms2 = miString(t1).split(8);
        const std::vector<std::string> s1(ms1.begin(), ms1.end());
        const std::vector<std::string> s2(ms2.begin(), ms2.end());

        EXPECT_EQ(ve1, s1);
        EXPECT_EQ(ve2, s2);
    }
    {
        const std::vector<std::string> s1 = miutil::split(t1, 2);
        const std::vector<std::string> s2 = miutil::split(t1, 8);
        EXPECT_EQ(ve1, s1);
        EXPECT_EQ(ve2, s2);
    }
    {
        EXPECT_EQ(0, miutil::split("", ":").size());
        EXPECT_EQ(1, miutil::split("one", ":").size());
    }
}

TEST(miStringTest, split_protected)
{
    const char t1[] = " (protected text) in a  \"string  ";

    const char* e1[] = { "(protected text)", "in", "a", "\"string" };
    const char* e2[] = { "(protected", "text)", "in", "a" };
    const std::vector<std::string> ve1(e1, boost::end(e1));
    const std::vector<std::string> ve2(e2, boost::end(e2));

    {
        const std::vector<miString> ms1 = miString(t1).split('(', ')');
        const std::vector<miString> ms2 = miString(t1).split('"', '"');
        const std::vector<std::string> s1(ms1.begin(), ms1.end());
        const std::vector<std::string> s2(ms2.begin(), ms2.end());

        EXPECT_EQ(ve1, s1);
        EXPECT_EQ(ve2, s2);
    }
    {
        const std::vector<std::string> s1 = miutil::split_protected(t1, '(', ')');
        const std::vector<std::string> s2 = miutil::split_protected(t1, '"', '"');
        
        EXPECT_EQ(ve1, s1);
        EXPECT_EQ(ve2, s2);
    }
}

TEST(miStringTest, is_number)
{
    EXPECT_TRUE(miutil::is_number(" 0.123 "));
    EXPECT_TRUE(miutil::is_number("+10."));
    EXPECT_TRUE(miutil::is_number("-.5 "));
    EXPECT_TRUE(miutil::is_number("  10.0E-1"));
    EXPECT_TRUE(miutil::is_number("-1 "));
    EXPECT_TRUE(miutil::is_number("-5.E3"));

    EXPECT_FALSE(miutil::is_number("no"));
    EXPECT_FALSE(miutil::is_number("-"));
    EXPECT_FALSE(miutil::is_number("+E1"));
    EXPECT_FALSE(miutil::is_number("E-1"));
}

TEST(miStringTest, is_int)
{
    EXPECT_TRUE(miutil::is_int(" 0123"));
    EXPECT_TRUE(miutil::is_int("10 "));
    EXPECT_TRUE(miutil::is_int("-5"));
    EXPECT_TRUE(miutil::is_int(" +2  "));

    EXPECT_FALSE(miutil::is_int("no"));
    EXPECT_FALSE(miutil::is_int(" -"));
    EXPECT_FALSE(miutil::is_int("+0.1"));
    EXPECT_FALSE(miutil::is_int(" 12345."));
    EXPECT_FALSE(miutil::is_int("- 12345"));
}


TEST(miStringTest, replace)
{
    std::string r1("miString is better");
    miutil::replace(r1, "miString", "std::string");
    EXPECT_EQ("std::string is better", r1);

    std::string r2("abcd");
    miutil::replace(r2, 'd', 'x');
    EXPECT_EQ("abcx", r2);
}

TEST(miStringTest, remove)
{
    std::string r1(" compact  but not readable  ");
    miutil::remove(r1, ' ');
    EXPECT_EQ("compactbutnotreadable", r1);
}

TEST(miStringTest, to_int)
{
    EXPECT_EQ(12, miutil::to_int("12"));
    EXPECT_EQ(-1, miutil::to_int("12 x", -1));
    EXPECT_EQ(-1, miutil::to_int("", -1));

    EXPECT_EQ(12, miutil::to_int("12.3"));
}

TEST(miStringTest, to_double)
{
  EXPECT_EQ(12.3, miutil::to_double("12.3"));
  EXPECT_EQ(12.3, miutil::to_double("12.3 "));
  EXPECT_EQ(-1,   miutil::to_double("12.3 x", -1));
  EXPECT_EQ(-1,   miutil::to_double("", -1));
  
  EXPECT_EQ(12.3, miutil::to_double("1.23E1"));

  EXPECT_FLOAT_EQ(0.1,  miutil::to_double("0.1"));
  EXPECT_FLOAT_EQ(0.2,  miutil::to_double("0.2"));
  EXPECT_FLOAT_EQ(0.01, miutil::to_double("0.01"));
  EXPECT_FLOAT_EQ(0.03, miutil::to_double("0.03"));
}

TEST(miStringTest, to_upper_lower)
{
    EXPECT_EQ(" RIKTIG", miutil::to_upper(" riKTiG"));
    EXPECT_EQ("$&/(HI)\"", miutil::to_upper("$&/(hi)\""));

    EXPECT_EQ("hello $&/()\"", miutil::to_lower("hELLo $&/()\""));

    EXPECT_EQ("aring\xC5\xE5 oslash\xD8\xF8", miutil::to_lower("aring\xC5\xE5 OSLASH\xD8\xF8"));
    EXPECT_EQ("ARING\xC5\xE5 OSLASH\xD8\xF8", miutil::to_upper("aring\xC5\xE5 OSLASH\xD8\xF8"));

    EXPECT_EQ("aring\xE5\xE5 oslash\xF8\xF8", miutil::to_lower_latin1("aring\xC5\xE5 OSLASH\xD8\xF8"));
    EXPECT_EQ("ARING\xC5\xC5 OSLASH\xD8\xD8", miutil::to_upper_latin1("aring\xC5\xE5 OSLASH\xD8\xF8"));

#if 0
    const char* utf8 = "oslash\xc3\x98\xc3\xb8 aring\xc3\x85\xc3\xa5 uuml\xc3\x9c\xc3\xbc";
    EXPECT_EQ("OSLASH\xc3\x98\xc3\x98 ARING\xc3\x85\xc3\x85 UUML\xc3\x9c\xc3\x9c", miutil::to_upper(utf8));
    EXPECT_EQ("oslash\xc3\xb8\xc3\xb8 aring\xc3\xa5\xc3\xa5 uuml\xc3\xbc\xc3\xbc", miutil::to_lower(utf8));
#endif
}
