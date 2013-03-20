/*
  libpuTools - Basic types/algorithms/containers

  $Id$

  Copyright (C) 2006 met.no

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

/* miClock

   Definition of miClock -- a class for manipulating clock times. In
   algebraic computations the number of seconds after midnight is used

   The default value of an miClock is `undef', and there is a function
   undef() to check for this state.

   Author: ØA/LBS DNMI/FoU 1997, 1998, 1999
   Part of the puTools kit. */

#ifndef __dnmi_miClock__
#define __dnmi_miClock__

#include <iosfwd>
#include <string>

namespace miutil{

class miClock {

  int Hour;          // hours since midnight (0,23)
  int Min;           // minutes after the hour (0,59)
  int Sec;           // seconds after the minute (0,59)
  long accSec;       // seconds after midnight (0,86399)

  void accSecToClock();
  void accSecToClock(long acc)
  { accSec=acc; accSecToClock(); }

  enum { MAXACC=86400 };

public:
  miClock(int h =-1,int m =-1,int s =-1)  // (-1,-1,-1) is the undef state
  { setClock(h,m,s); }
  explicit miClock(const char* s)     // construct clock time from "hh:mm:ss"
  { setClock(s); }
  explicit miClock(const std::string& s) // ---------------"-------------------
  { setClock(s); }

  bool undef() const
  { return (accSec==-3661); }

  static bool isValid(int, int, int);
  static bool isValid(const std::string&);

  void setClock(int, int, int);
  void setClock(const std::string&);

  int hour() const
  { return Hour; }
  int min() const
  { return Min; }
  int sec() const
  { return Sec; }

  std::string isoClock() const;
  std::string isoClock(bool withmin, bool withsec) const;

  friend int operator==(const miClock& lhs, const miClock& rhs)
  { return (lhs.accSec==rhs.accSec); }
  friend int operator!=(const miClock& lhs, const miClock& rhs)
  { return (lhs.accSec!=rhs.accSec); }
  friend int operator>(const miClock& lhs, const miClock& rhs)
  { return (lhs.accSec>rhs.accSec); }
  friend int operator<(const miClock& lhs, const miClock& rhs)
  { return (lhs.accSec<rhs.accSec); }
  friend int operator>=(const miClock& lhs, const miClock& rhs)
  { return (lhs.accSec>=rhs.accSec); }
  friend int operator<=(const miClock& lhs, const miClock& rhs)
  { return (lhs.accSec<=rhs.accSec); }

  void addSec(int =1);  // add seconds
  void addMin(int =1);  // add minutes
  void addHour(int =1); // add hours

  static int hourDiff(const miClock&, const miClock&);
  static int minDiff(const miClock&, const miClock&);
  static int secDiff(const miClock&, const miClock&);

  static miClock oclock();

  std::string format(const std::string&) const;

  friend std::ostream& operator<<(std::ostream& output, const miClock& c)
  { return output << c.isoClock(); }
};

}
#endif
