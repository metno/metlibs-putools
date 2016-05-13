/*
  Diana - A Free Meteorological Visualisation Tool

  Copyright (C) 2006-2016 met.no

  Contact information:
  Norwegian Meteorological Institute
  Box 43 Blindern
  0313 OSLO
  NORWAY
  email: diana@met.no

  This file is part of Diana

  Diana is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Diana is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Diana; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TimeFilter.h"

#include <stdexcept>
#include <sstream>

namespace /*anonymous*/ {
bool assign_pos(std::string::size_type& pos, std::string::size_type new_pos)
{
  if (pos != std::string::npos)
    return false;
  pos = new_pos;
  return true;
}

std::string::size_type replace_first_if(std::string& s, const char* thys, int size)
{
  const std::string::size_type idx = s.find(thys);
  if (idx != std::string::npos) {
    for (int i=0; i<size; ++i)
      s[idx + i] = '?';
  }
  return idx;
}

void parse_time_int(const std::string& text, std::string::size_type idx, size_t count, std::string::size_type offset, int& value)
{
  if (idx == std::string::npos)
    return;

  idx += offset;

  if (count < 1 || idx+count > text.size())
    throw std::runtime_error("end of string");

  value = 0;
  for (size_t i=0; i<count; ++i, ++idx) {
    char ch = text[idx];
    if (ch < '0' || ch > '9')
      throw std::runtime_error("no digit");
    value = 10*value + (ch - '0');
  }
}
} /*anonymous namespace*/


namespace miutil {

TimeFilter::TimeFilter()
{
  reset();
}

TimeFilter::TimeFilter(std::string& filename)
{
  initFilter(filename);
}

void TimeFilter::reset()
{
  yyyy = yy = mm = dd = HH = MM = SS = std::string::npos;
}

bool TimeFilter::initFilter(std::string& filename)
{

  reset();

  if (filename == "OFF")
    return false;

  if ( filename.find("[") == std::string::npos || filename.find("]") == std::string::npos ) {
    return false;
  }

  noSlash = (filename.find("/") == std::string::npos);

  try {
    filename = parse(filename);
    return ok();
  } catch (std::exception& e) {
    reset();
    return false;
  }
}

bool TimeFilter::ok() const
{
  return (dd != std::string::npos && mm != std::string::npos &&
      (yy != std::string::npos || yyyy != std::string::npos));
}


std::string TimeFilter::parse(const std::string& filename)
{
  std::ostringstream pattern;

  size_t pos = 0;
  while (pos < filename.size()) {
    size_t bracket_open = filename.find("[", pos);
    if (bracket_open == std::string::npos)
      break;
    if (pos+4 > filename.size())
      throw std::runtime_error("no space for pattern after '['");

    size_t bracket_close = filename.find("]", bracket_open + 3);
    if (pos == std::string::npos)
      throw std::runtime_error("no closing ']'");

    pattern << filename.substr(pos, bracket_open - pos);

    size_t pat = bracket_open + 1;
    while (pat < bracket_close) {
      const char pat0 = filename[pat];
      if (pat0 != 'y' && pat0 != 'm' && pat0 != 'd' && pat0 != 'H' && pat0 != 'M' && pat0 != 'S') {
        // ignore unknown pattern char
        pattern << pat0;
        pat += 1;
        continue;
      }

      if (pat0 != filename[pat+1])
        throw std::runtime_error("invalid pattern");

      int n = 2;
      const size_t new_pos = pattern.tellp();
      if (pat0 == 'm' && assign_pos(mm, new_pos)) {
        // ok
      } else if (pat0 == 'd' && assign_pos(dd, new_pos)) {
        // ok
      } else if (pat0 == 'H' && assign_pos(HH, new_pos)) {
        // ok
      } else if (pat0 == 'M' && assign_pos(MM, new_pos)) {
        // ok
      } else if (pat0 == 'S' && assign_pos(SS, new_pos)) {
        // ok
      } else if (pat0 == 'y' && yy == std::string::npos && yyyy == std::string::npos) {
        if (pat + 3 < bracket_close && filename[pat+2] == 'y' && filename[pat+3] == 'y') {
          yyyy = new_pos;
          n = 4;
        } else {
          yy = new_pos;
        }
      } else {
        throw std::runtime_error("pattern error");
      }
      for (int i=0; i<n; ++i)
        pattern << '?';
      pat += n;
    }
    pos = bracket_close + 1;
  }
  if (pos < filename.size())
    pattern << filename.substr(pos);

  return pattern.str();
}

bool TimeFilter::getTime(const std::string& name, miutil::miTime &time) const
{
  if (!ok() || name.empty())
    return false;

  int offset = 0;
  if (noSlash) {
    const std::string::size_type slash = name.find_last_of("/");
    if (slash != std::string::npos)
      offset = slash + 1;
  }

  try {
    int year = 0, month, day, hour=12, minute = 0, second = 0;

    parse_time_int(name, HH, 2, offset, hour);
    parse_time_int(name, MM, 2, offset, minute);
    parse_time_int(name, SS, 2, offset, second);

    parse_time_int(name, dd, 2, offset, day);
    parse_time_int(name, mm, 2, offset, month);

    if (yyyy != std::string::npos) {
      parse_time_int(name, yyyy, 4, offset, year);
    } else {
      parse_time_int(name, yy, 2, offset, year);
      if (year > 50)
        year += 1900;
      else
        year += 2000;
    }

    miutil::miTime t(year, month, day, hour, minute, second);
    if (!t.undef()) {
      std::swap(t, time);
      return true;
    }
  } catch (std::exception& e) {
    // nothing
  }
  return false;
}

std::string TimeFilter::getTimeStr(const std::string& filename) const
{
  if (ok()) {
    miutil::miTime time;
    if (getTime(filename, time))
      return time.isoTime("T");
  }
  return std::string();
}

} // namespace miutil
