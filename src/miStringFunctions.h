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

#ifndef metlibs_puTools_miStringFunctions
#define metlibs_puTools_miStringFunctions

#include <algorithm>
#include <climits> 
#include <cmath>
#include <string>
#include <vector>

namespace miutil {

extern const char whitespaces[];

inline int count_char(const std::string& text, char count_me)
{ return std::count(text.begin(), text.end(), count_me); }

inline std::string from_c_str(const char* s)
{ return s ? std::string(s) : std::string(); }

std::string from_number(const int d, const int width=0, const char fill='0');
std::string from_number(const double d, const int prec =-1);
std::string from_number(const float d, const int prec =-1);

void trim(std::string& text, bool left=true, bool right=true, const char* wspace=whitespaces);
inline std::string trimmed(const std::string& text, bool left=true, bool right=true, const char* wspace=whitespaces)
{ std::string t(text); trim(t, left, right, wspace); return t; }
void trim_remove_empty(std::vector<std::string>& strings);

std::vector<std::string> split(const std::string& text, int nos, const char* separator_chars=whitespaces, const bool clean=true);
inline std::vector<std::string> split(const std::string& text, const char* separator_chars=whitespaces, const bool clean=true)
{ return split(text, 0, separator_chars, clean); }
inline std::vector<std::string> split(const std::string& text, const std::string& separator_chars, const bool clean=true)
{ return split(text, 0, separator_chars.c_str(), clean); }
std::vector<std::string> split_protected(const std::string& text, const char left, const char right,
                                         const char* separator_chars=whitespaces, const bool clean=true);

void remove(std::string& text, const char c);
void replace(std::string& text, const char thys, const char that);
void replace(std::string& text, const std::string& thys, const std::string& that);

inline bool contains(const std::string& haystack, const std::string& needle)
{ return haystack.find(needle) != std::string::npos; }

bool is_number(const std::string& text);
bool is_int(const std::string& text);

int to_int(const std::string& text, const int undefined=INT_MIN);
long to_long(const std::string& text, const long undefined=LONG_MIN);
float to_float(const std::string& text, const float undefined=NAN);
double to_double(const std::string& text, const double undefined=NAN);

std::string to_lower(const std::string& text);
std::string to_upper(const std::string& text);

std::string to_upper_latin1(const std::string& text);
std::string to_lower_latin1(const std::string& text);

/** Appends b to a, with separator inbetween if a is not empty; returns a if b is empty. */
std::string appended(const std::string& a, const std::string& separator, const std::string& b);
void appendTo(std::string& a, const std::string& separator, const std::string& b);

} // namespace miutil

#endif // metlibs_puTools_miStringFunctions
