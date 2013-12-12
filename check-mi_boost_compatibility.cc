
#define MI_BOOST_COMPATIBILITY_FORCE_SHARED_PTR
#define MI_BOOST_COMPATIBILITY_FORCE_MAP_ADAPTORS
#define MI_BOOST_COMPATIBILITY_FORCE_FIND_IF
#include "mi_boost_compatibility.hh"

#include <boost/foreach.hpp>
#include <map>
#include <string>

#include <gtest/gtest.h>

namespace /*anonymous*/ {
struct Hello
{
  static int what;
  Hello()
    { what = 0; }
  Hello(int)
    { what = 1; }
  Hello(const char*)
    { what = 2; }
};

int Hello::what = -1;
} // anonymous namespace

TEST(mi_boost_compatibility_test, make_shared)
{
    miutil::make_shared<Hello>("world");
    EXPECT_EQ(2, Hello::what);
    miutil::make_shared<Hello>();
    EXPECT_EQ(0, Hello::what);
    miutil::make_shared<Hello>(2);
    EXPECT_EQ(1, Hello::what);
}

TEST(mi_boost_compatibility_test, map_keys_values)
{
  const size_t N = 4;
  const int keys[N] = { 1, 3, 5, 7 };
  const std::string values[] = { "one", "three", "five", "seven" };

  typedef std::map<int, std::string> mymap_t;
  mymap_t mymap;
  for (size_t i=0; i<N; ++i)
    mymap[keys[i]] = values[i];
  const mymap_t mymap_c(mymap);
  
  int i1 = 0;
  BOOST_FOREACH(int k, miutil::adaptors::keys(mymap)) {
    EXPECT_EQ(keys[i1++], k);
  }
  EXPECT_EQ(N, i1);
  
  int i2 = 0;
  BOOST_FOREACH(int k, miutil::adaptors::keys(mymap_c)) {
    EXPECT_EQ(keys[i2++], k);
  }
  EXPECT_EQ(N, i2);
  
  int i3 = N;
  BOOST_REVERSE_FOREACH(int k, miutil::adaptors::keys(mymap)) {
    EXPECT_EQ(keys[--i3], k);
  }
  EXPECT_EQ(0, i3);
  
  int i4 = 0;
  BOOST_FOREACH(const std::string& v, miutil::adaptors::values(mymap)) {
    EXPECT_EQ(values[i4++], v);
  }
  EXPECT_EQ(N, i4);
  
  int i5 = N;
  BOOST_REVERSE_FOREACH(const std::string& v, miutil::adaptors::values(mymap)) {
    EXPECT_EQ(values[--i5], v);
  }
  EXPECT_EQ(0, i5);

  typedef miutil::adaptors::map<mymap_t>::values mymap_values_t;
  mymap_values_t mv(mymap);
  for (mymap_values_t::reverse_iterator it=mv.rbegin(); it != mv.rend(); ++it) {
    *it += " fish";
  }

  int i6 = 0;
  typedef miutil::adaptors::map<const mymap_t>::keys mymap_keys_t;
  const mymap_keys_t mk1(mymap_c);
  for (mymap_keys_t::const_iterator it=mk1.begin(); it != mk1.end(); ++it) {
    EXPECT_EQ(keys[i6++], *it);
  }
  EXPECT_EQ(N, i6);

  int i7 = 0;
  const mymap_keys_t mk2(mymap);
  for (mymap_keys_t::const_iterator it=mk2.begin(); it != mk2.end(); ++it) {
    EXPECT_EQ(values[i7++] + " fish", mymap.at(*it));
  }
  EXPECT_EQ(N, i7);
}

namespace {
typedef std::set<std::string> string_s;
struct eq_string {
  const std::string& s;
  eq_string(const std::string& S) : s(S) { }
  bool operator() (const std::string& o) const
    { return s == o; }
};
}

TEST(mi_boost_compatibility_test, find_if)
{
  string_s hello;
  hello.insert("hello");
  hello.insert("world");

  const string_s::iterator it1 = miutil::find_if(hello, eq_string("world"));
  EXPECT_EQ(++hello.begin(), it1);
  const string_s::const_iterator it2 = miutil::find_if(hello, eq_string("hello"));
  EXPECT_EQ(hello.begin(), it2);

  const string_s hello_c(hello);
  const string_s::const_iterator it3 = miutil::find_if(hello_c, eq_string("moon"));
  EXPECT_EQ(hello_c.end(), it3);
}
