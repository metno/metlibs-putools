/* -*- c++ -*-
  libpuTools - Basic types/algorithms/containers
  
  Copyright (C) 2006-2013 met.no

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

#ifndef __dnmi_miString__
#define __dnmi_miString__

#include "miStringFunctions.h"
#include "puCtools/deprecated.h"
#include <sstream>
#include "puAlgo.h"

namespace miutil {

class miString : public std::string
{
public:
  miString()
    : std::string() {}
  METLIBS_DEPRECATED(miString(const char* s), "use 'miutil::from_c_str(s)' if 0-protection is required");
  miString(const std::string& s)
    : std::string(s) {}

  METLIBS_DEPRECATED(METLIBS_CONCAT(explicit miString(const int    d, const int width=0, const char fill='0')), "use 'from_number(...)'");
  METLIBS_DEPRECATED(METLIBS_CONCAT(explicit miString(const double d, const int prec =-1)), "use 'from_number(...)'");
  METLIBS_DEPRECATED(METLIBS_CONCAT(explicit miString(const float  d, const int prec =-1)), "use 'from_number(...)'");

  METLIBS_DEPRECATED(const char* cStr() const, "use c_str() directly")
  { return c_str(); }

  METLIBS_DEPRECATED(bool contains(const miString& s) const, "use 'miutil::contains(haystack, needle)' directly")
  { return miutil::contains(*this, s); }

  METLIBS_DEPRECATED(bool exists() const, "use 'not empty()'")
  { return !empty(); }

  METLIBS_DEPRECATED(METLIBS_CONCAT(void trim(bool right=true, bool left=true, const miString=whitespaces)), "use 'miutil::split(...)'");
  METLIBS_DEPRECATED(METLIBS_CONCAT(void rtrim(const miString ws=whitespaces)), "use 'miutil::trim(...)'")
        { miutil::trim(*this, false, true, ws.c_str()); }
  METLIBS_DEPRECATED(METLIBS_CONCAT(inline void ltrim(const miString ws=whitespaces)), "use 'miutil::trim(...)'")
        { miutil::trim(*this, true, false, ws.c_str()); }

  METLIBS_DEPRECATED(int countChar(const char c) const, "use miutil::count_char")
  { return count_char(*this, c); }

  METLIBS_DEPRECATED(void remove(const char), "use 'miutil::remove(...)'");
  METLIBS_DEPRECATED(METLIBS_CONCAT(miString replace(const char, const char          ) const), "use 'miutil::replace(...)'");
  METLIBS_DEPRECATED(METLIBS_CONCAT(void     replace(const miString&, const miString&)), "use 'miutil::replace(...)'");

  METLIBS_DEPRECATED(METLIBS_CONCAT(std::vector<miString> split(const miString =whitespaces, const bool =true) const), "use 'miutil::split(...)'");
  METLIBS_DEPRECATED(METLIBS_CONCAT(std::vector<miString> split(const char, const bool =true) const), "use 'miutil::split(...)'");

  // nos = max number of splits, 0 = split all elements

  METLIBS_DEPRECATED(METLIBS_CONCAT(std::vector<miString> split(int nos, const miString =whitespaces, const bool =true) const), "use 'miutil::split(...)'");
  METLIBS_DEPRECATED(METLIBS_CONCAT(std::vector<miString> split(int nos, const char, const bool =true) const), "use 'miutil::split(...)'");

  // left/right border to protect strings inside the string () "" etc.

  METLIBS_DEPRECATED(METLIBS_CONCAT(std::vector<miString> split(const char leftborder, const char rightborder,
                                                const miString =whitespaces, 
                                                const bool =true) const), "use 'miutil::split(...)'");


  METLIBS_DEPRECATED(METLIBS_CONCAT(template< template< typename T, typename ALLOC = std::allocator<T> > class C, typename T>
                    inline void join(const C<T>& j, const miString delimiter=" ")), "use 'boost::algorithm::join(j, delimiter)'");

  METLIBS_DEPRECATED(METLIBS_CONCAT(template< template< typename T, typename  COMPARE = std::less<T>, typename ALLOC = std::allocator<T> > class C, typename T>
                    inline void join(const C<T>& j, const miString delimiter=" ")), "use 'boost::algorithm::join(j, delimiter)'");

  METLIBS_DEPRECATED(METLIBS_CONCAT(miString upcase(  int start=0, int len=0) const), "use miutil::to_upper or a charset-aware upcase function");
  METLIBS_DEPRECATED(METLIBS_CONCAT(miString downcase(int start=0, int len=0) const), "use miutil::to_lower or a charset-aware downcase function");

  METLIBS_DEPRECATED(bool isNumber() const, "use 'miutil::is_number(...)' directly");
  METLIBS_DEPRECATED(bool isInt()    const, "use 'miutil::is_int(...)' directly");

  METLIBS_DEPRECATED(int    toInt(    int    undefined=INT_MIN  ) const, "use 'to_int(...)'");
  METLIBS_DEPRECATED(float  toFloat(  float  undefined=NAN      ) const, "use 'to_double(...)'");
  METLIBS_DEPRECATED(double toDouble( double undefined=NAN      ) const, "use 'to_double(...)'");
  METLIBS_DEPRECATED(long   toLong(   long   undefined=LONG_MIN ) const, "use 'to_long(...)'");

};

template< template< typename T, typename ALLOC = std::allocator<T> > class C, typename T>
inline void miString::join(const C<T>& j, const miString delimiter)
{
    bool first=true;
    std::ostringstream ost;
    typename C<T>::const_iterator itr= j.begin();
    for(;itr!=j.end();itr++){
      ost << (first ? "" : delimiter ) << *itr;
      first = false;
    }
    *this=ost.str();
}

template< template< typename T, typename  COMPARE = std::less<T>, typename ALLOC = std::allocator<T> > class C, typename T>
inline void miString::join(const C<T>& j, const miString delimiter)
{
    bool first=true;
    std::ostringstream ost;
    typename C<T>::const_iterator itr= j.begin();
    for(;itr!=j.end();itr++){
      ost << (first ? "" : delimiter ) << *itr;
      first = false;
    }
    *this=ost.str();
}

} // namespace miutil

#endif
