// -*- c++ -*-
/*
  Diana - A Free Meteorological Visualisation Tool

  Copyright (C) 2013-2016 met.no

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
#ifndef TimeFilter_h
#define TimeFilter_h

#include "miTime.h"

namespace miutil {

/**
  \brief get time info from file name
*/
class TimeFilter {
public:
  TimeFilter();
  TimeFilter(std::string& filename);

  /// remember time info, return filename with time info replaced by '?'s
  bool initFilter(std::string& filename);

  void reset();

  /// returns true if initFilter found timeinfo in filename
  bool ok() const;

  /// find time from filename
  bool getTime(const std::string& name, miutil::miTime& t) const;

  std::string getTimeStr(const std::string& name) const;

private:
  std::string parse(const std::string& filename);

private:
  std::string::size_type yyyy,yy,mm,dd,HH,MM,SS;
  bool noSlash;
};

} // namespace miutil

#endif // TimeFilter_h
