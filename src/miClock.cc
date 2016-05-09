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


// miClock.cc : Definitions for miClock class
//
// Authors: �A/LBS 1997:   Creation
//          LBS Nov. 1998: Changed the default state from being the
//                         current UTC to an `undefined' state.

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "miClock.h"
#include "miString.h"

#include <iostream>
#include <sstream>
#include <iomanip>

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;
using namespace miutil;

static inline void warning(const std::string& s)
{
  std::cerr << "Warning: miClock::" << s << std::endl;
}

void
miutil::miClock::accSecToClock()
{
  accSec=(accSec+MAXACC)%MAXACC;   // caution if accSec < 0 or > MAXACC
  long acc=accSec;
  Hour=acc/3600;
  acc-=Hour*3600;
  Min=acc/60;
  acc-=Min*60;
  Sec=acc;
}

void
miutil::miClock::setClock(int h, int m, int s)
{
  // if any of the arguments have imposible values, prepare for undef
  if (!isValid(h,m,s)){
    std::string w="setClock: Illegal clock HH:MM:SS ("
        + miutil::from_number(h)+":"+miutil::from_number(m)+":"+miutil::from_number(s)+")";
    warning(w);
    Hour   = -1;
    Min    = -1;
    Sec    = -1;
    accSec = Hour*3600+Min*60+Sec;
  }
  else {
    Hour=h;
    Min=m;
    Sec=s;
    accSec=Hour*3600+Min*60+Sec;  // seconds since 00:00:00
  }
}

// converts "hh:mm:ss" to miClock
void
miutil::miClock::setClock(const std::string& str)
{
  int h=0, m=0, s=0;
  std::string str_=str;
  miutil::remove(str_,':');
  sscanf(str_.c_str(), "%2d%2d%2d",&h, &m, &s);
  if (!isValid(str_))
    warning("setClock: Error in format. Should be `HH:MM:SS' or `HHMMSS' (" + str + ")");
  else
    sscanf(str_.c_str(), "%2d%2d%2d",&h, &m, &s);
  setClock(h,m,s);
}

bool
miutil::miClock::isValid(int h, int m, int s)
{
  if (h<0 || h>23 || m<0 || m>59 || s<0 || s>59)
    if (!(h==-1 && m==-1 && s==-1)) // These `illegal' values are allowed
      return false;
  return true;
}

bool
miutil::miClock::isValid(const std::string& str)
{
  int h=0,m=0,s=0;
  std::string str_=str;
  miutil::remove(str_,':');
  sscanf(str_.c_str(), "%2d%2d%2d",&h, &m, &s);
  return isValid(h,m,s);
}

// Format ISO "hh:mm:ss" string
std::string
miutil::miClock::isoClock() const
{
  ostringstream ost;

  if (undef()) {
    warning("isoClock: undefined time");
    ost << "--:--:--";
  }
  else
    ost << setw(2) << setfill('0') << Hour << ':'
	<< setw(2) << setfill('0') << Min  << ':'
	<< setw(2) << setfill('0') << Sec;

  return ost.str();
}

// Format 'almost' ISO "hh[:mm:ss]" string
std::string
miutil::miClock::isoClock(bool withmin, bool withsec) const
{
  ostringstream ost;

  if (withsec && !withmin) withmin= true;

  if (undef()) {
    warning("isoClock: undefined time");
    ost << "--";
    if (withmin) ost << ":--";
    if (withsec) ost << ":--";
  }
  else {
    ost << setw(2) << setfill('0') << Hour;
    if (withmin) ost << ':' << setw(2) << setfill('0') << Min;
    if (withsec) ost << ':' << setw(2) << setfill('0') << Sec;
  }

  return ost.str();
}

void
miutil::miClock::addSec(int s)       // add seconds
{
  if (undef()) {
    warning("addSec: Clock is undefined; can't add seconds");
    return;
  }

  accSec+=s;
  accSecToClock();
}

void
miutil::miClock::addMin(int m)       // add minutes
{
  if (undef()) {
    warning("addMin: Clock is undefined; can't add minutes");
    return;
  }

  accSec+=60*m;
  accSecToClock();
}

void
miutil::miClock::addHour(int h)      // add hours
{
  if (undef()) {
    warning("addHour: Clock is undefined; can't add hours");
    return;
  }

  accSec+=3600*h;
  accSecToClock();
}

int
miutil::miClock::hourDiff(const miClock& lhs, const miClock& rhs)
{
  if (lhs.undef() || rhs.undef()) {
    warning("hourDiff: One clock is undefined. Can't subtract hours.");
    return 0;
  }
  return (lhs.accSec-rhs.accSec)/3600;
}

int
miutil::miClock::minDiff(const miClock& lhs, const miClock& rhs)
{
  if (lhs.undef() || rhs.undef()) {
    warning("minDiff: One clock is undefined. Can't subtract minutes.");
    return 0;
  }
  return (lhs.accSec-rhs.accSec)/60;
}

int
miutil::miClock::secDiff(const miClock& lhs, const miClock& rhs)
{
  if (lhs.undef() || rhs.undef()) {
    warning("secDiff: One clock is undefined. Can't subtract seconds.");
    return 0;
  }
  return lhs.accSec-rhs.accSec;
}

miutil::miClock
miutil::miClock::oclock()
{
  time_t curTime_t=time(0);
  struct tm* curTime=gmtime(&curTime_t);

  return miClock(curTime->tm_hour, curTime->tm_min, curTime->tm_sec);
}

std::string
miutil::miClock::format(const std::string& newClock) const
{
  if(undef())
    return newClock;


  bool pm = (Hour < 1 || Hour > 12 ? true : false );

  int tH =  ( Hour ? Hour : 24 ) - (pm ? 12 : 0);

  std::string c(newClock);

  miutil::replace(c, "%X","%H:%M:%S");         /// %X 24-hour (hh:mm:ss)
  miutil::replace(c, "%T","%H:%M:%S");         /// %X 24-hour (hh:mm:ss)
  miutil::replace(c, "%r","%I:%M:%S %p");      /// %r 12-hour (hh:mm:ss [AP]M)

  if (miutil::contains(c, "$midnight24")){   /// show midnight as 24:00:00
    miutil::replace(c, "%H","24");
    miutil::replace(c, "%k","24");
    miutil::replace(c, " $midnight24","");
    miutil::replace(c, "$midnight24 ","");
    miutil::replace(c, "$midnight24","");
  }

  miutil::replace(c, "%H", miutil::from_number(Hour,2));   /// %H hour (00..23)
  miutil::replace(c, "%I", miutil::from_number(tH,2));     /// %I hour (01..12)
  miutil::replace(c, "%k", miutil::from_number(Hour));     /// %k hour ( 0..23)
  miutil::replace(c, "%l", miutil::from_number(tH));       /// %l hour ( 1..12)
  miutil::replace(c, "%M", miutil::from_number(Min,2));    /// %M min  (00..59)

  if(miutil::contains(c, "$30M") ){
    miutil::replace(c, "$30M", (Min < 30 ? "00" : "30" )); /// SIGMET SPECIAL
  }

  miutil::replace(c, "%p",(pm ? "PM" : "AM")); /// %p locale's AM or PM
  miutil::replace(c, "%S", miutil::from_number(Sec,2));    /// %S second (00..60)

  return c;
}







