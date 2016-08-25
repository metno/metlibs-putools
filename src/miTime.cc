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


// miTime.cc : Definitions for miTime class
// ØA, 1997

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "miTime.h"
#include "miString.h"

#include <cstdio>
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;
using namespace miutil;
using namespace boost::posix_time;

namespace /*anonymous*/ {
bool show_message()
{
  static const int message_counter_max = 100;
  static int message_counter = 0;
  if (message_counter < message_counter_max) {
    message_counter += 1;
    return true;
  } else {
    return false;
  }
}

inline void warning(const std::string& s)
{
  if (show_message())
    std::cerr << "Warning: miTime::" << s << std::endl;
}

inline void invalid(const std::string& s)
{
  if (show_message())
    std::cerr << "Warning: miTime::setTime: (" << s << ") is not a valid time" << std::endl;
}
} // anonymous namespace

/* Construct miTime from UNIX time (this function has a Y2038 problem
   with current UNIX implementaions) */
miutil::miTime::miTime(const time_t& t)
{
  struct tm *gmt=gmtime(&t);

  if (gmt) {
    Date.setDate(gmt->tm_year+1900, gmt->tm_mon+1, gmt->tm_mday);
    Clock.setClock(gmt->tm_hour, gmt->tm_min, gmt->tm_sec);
  }
}

// make time from "yyyy-mm-dd hh:mm:ss", "yyyy-mm-dd"
// from yyyymmddhhmmss, yyyymmddhhmm, yyyymmddhh or yyyymmdd
void
miutil::miTime::setTime(const std::string& st)
{
  vector<std::string> t;
  std::string str=st;
  if(miutil::contains(str, "Z"))
    miutil::replace(str, "Z", "");

  miutil::trim(str);

  if(miutil::contains(str, "T"))
    t=miutil::split(str, "T");
  else
    t=miutil::split(str);

  if (t.size()>=2) {
    Date.setDate(t[0]);
    Clock.setClock(t[1]);
    return;
  }

  int yy, mm, dd;
  int h   = 0;
  int m   = 0;
  int s   = 0;
  int len = str.length();

  if(miutil::contains(str, "-")){
    Date.setDate(str);
    Clock.setClock(h,m,s);
    return;
  }

  if(len==14){
    if(sscanf(str.c_str(), "%4d%2d%2d%2d%2d%2d", &yy,&mm,&dd, &h,&m,&s)!=6){
      invalid(str);
      return;
    }
  }else if(len==12){
    if(sscanf(str.c_str(), "%4d%2d%2d%2d%2d"   , &yy,&mm,&dd, &h,&m)!=5){
      invalid(str);
      return;
    }
  }else if(len==10){
    if(sscanf(str.c_str(), "%4d%2d%2d%2d"      , &yy,&mm,&dd, &h)!=4){
      invalid(str);
      return;
    }
  }else if(len==8){
    if(sscanf(str.c_str(), "%4d%2d%2d"         , &yy,&mm,&dd)!=3){
      invalid(str);
      return;
    }
  }else{
    invalid(str);
    return;
  }

  Date.setDate(yy,mm,dd);
  Clock.setClock(h,m,s);
}


bool
miutil::miTime::isValid(int y, int m, int d, int h, int min, int s)
{
  if(!miutil::miClock::isValid(h,min,s))
    return false;
  return miutil::miDate::isValid(y,m,d);
}

bool
miutil::miTime::isValid(const std::string& st)
{
  vector<std::string> t=miutil::split(st);
  if ( t.size() < 2 ) {
    t=miutil::split(st, "T");
    if ( t.size() < 2  ) {
      t=miutil::split(st, "t");
    }
  }

  if (t.size()>=2) {
    if(!miutil::miDate::isValid(t[0]))
      return false;
    return miutil::miClock::isValid(t[1]);
  }

  int yy, mm, dd;
  int h  = 0;
  int m  = 0;
  int s  = 0;

  std::string str=st;

  miutil::trim(str);
  int len = str.length();

  if(miutil::contains(str, "-"))
    return miutil::miClock::isValid(str);

  if(len==14){
    if(sscanf(str.c_str(), "%4d%2d%2d%2d%2d%2d", &yy,&mm,&dd, &h,&m,&s)!=6)
      return false;
  }else if(len==12){
    if(sscanf(str.c_str(), "%4d%2d%2d%2d%2d"   , &yy,&mm,&dd, &h,&m)!=5)
      return false;
  }else if(len==10){
    if(sscanf(str.c_str(), "%4d%2d%2d%2d"      , &yy,&mm,&dd, &h)!=4)
      return false;
  }else if(len==8) {
    if(sscanf(str.c_str(), "%4d%2d%2d"         , &yy,&mm,&dd)!=3)
      return false;
  }else
    return false;

  return isValid(yy,mm,dd,h,m,s);
}

std::string
miutil::miTime::isoTime(const std::string& delim) const
{
  if (undef()){
    warning("isoTime: Object is not initialised.");
    return std::string("0000-00-00") + delim + std::string("--:--:--");
  }
  std::string _format = std::string("%Y-%m-%d") + delim + std::string("%H:%M:%S");
  return format(*this, _format);
  //return Date.isoDate() + delim + Clock.isoClock();
}

std::string
miutil::miTime::isoTime(bool withmin, bool withsec) const
{
  if (undef()){
    warning("isoTime: Object is not initialised.");
    return std::string("0000-00-00 ") + std::string("--:--:--");
  }
  std::string _format;
  if (withmin && withsec)
	_format= std::string("%Y-%m-%d") + " " + std::string("%H:%M:%S");
  if (!withmin && !withsec)
	_format= std::string("%Y-%m-%d") + " " + std::string("%H");
  if (withmin && !withsec)
    _format= std::string("%Y-%m-%d") + " " + std::string("%H:%M");
  return format(*this, _format);
}

void
miutil::miTime::addDay(int d)
{
  if (undef()) {
    warning("addDay: Can't add days. Object is not initialised.");
    return;
  }

  Date.addDay(d);
}

void
miutil::miTime::addHour(int h)      // add hours
{
  if (undef()) {
    warning("addHour: Can't add hours. Object is not initialised.");
    return;
  }

  h+=Clock.hour();
  if ((h>=0) && (h/24>0)) {
    addDay(h/24);
    h%=24;
  }
  else if (h<0) {
    addDay((h%24==0?0:-1)+h/24);
    h=(24-abs(h%24))%24;
  }
  Clock.setClock(h, Clock.min(), Clock.sec());
}

void
miutil::miTime::addMin(int m)      // add minutes
{
  if (undef()) {
    warning("addMin: Can't add minutes. Object is not initialised.");
    return;
  }

  m+=Clock.min();
  if ((m>=0) && (m/60>0)) {
    addHour(m/60);
    m%=60;
  }
  else if (m<0) {
    addHour((m%60==0?0:-1)+m/60);
    m=(60-abs(m%60))%60;
  }
  Clock.setClock(Clock.hour(), m, Clock.sec());
}

void
miutil::miTime::addSec(int s)      // add seconds
{
  if (undef()) {
    warning("addSec: Can't add seconds. Object is not initialised.");
    return;
  }

  s+=Clock.sec();
  if ((s>=0) && (s/60>0)) {
    addMin(s/60);
    s%=60;
  }
  else if (s<0) {
    addMin((s%60==0? 0: -1) + s/60);
    s=(60-abs(s%60))%60;
  }
  Clock.setClock(Clock.hour(), Clock.min(), s);
}

int
miutil::miTime::hourDiff(const miTime& lhs, const miTime& rhs)
{
  if (lhs.undef() || rhs.undef()) {
    warning("hourDiff: One date is undefined. Can't subtract hours.");
    return 0;
  }

  int dateDiff = lhs.Date-rhs.Date;
  if (lhs.hour()<rhs.hour())
    --dateDiff;
  return 24*(dateDiff)+((lhs.Clock.hour()+24-rhs.Clock.hour())%24);
}

int
miutil::miTime::minDiff(const miTime& lhs, const miTime& rhs)
{
  if (lhs.undef() || rhs.undef()) {
    warning("minDiff: One date is undefined. Can't subtract minutes.");
    return 0;
  }

  int hDiff=hourDiff(lhs,rhs);
  if (lhs.min()<rhs.min())
    --hDiff;
  return hDiff*60+((lhs.Clock.min()+60-rhs.Clock.min())%60);
}

int
miutil::miTime::secDiff(const miTime& lhs, const miTime& rhs)
{
  if (lhs.undef() || rhs.undef()) {
    warning("secDiff: One date is undefined. Can't subtract seconds.");
    return 0;
  }

  int mDiff=minDiff(lhs,rhs);
  if (lhs.sec()<rhs.sec())
    --mDiff;
  return mDiff*60+((lhs.Clock.sec()+60-rhs.Clock.sec())%60);
}

// returns one for daylight saving time. else 0

int
miutil::miTime::dst() const
{
  if(undef())
    return 0;

  int lsi;
  miDate sun(year(),month(),Date.daysInMonth());

  if(month() > 3  && month() < 10) return 1;
  if(month() > 10 || month() < 3 ) return 0;

  while(1) {
    if(!sun.dayOfWeek()){
      lsi = sun.day();
      break;
    }
    sun.addDay(-1);
  }

  if(month() == 10 ) {
    if(day() < lsi) return 1;
    if(day() == lsi && hour() < 3 )  return 1;
  }

  if(month() == 3) {
    if(day() > lsi)
      return 1;
    if(day() == lsi && hour() > 2 )
      return 1;
  }
  return 0;
}

int
miutil::miTime::timezone(const std::string& stz)
{

  if(stz=="UTC"   ) return  0;
  if(stz=="GMT"   ) return  0;
  if(stz=="CET"   ) return  1;
  if(stz=="EET"   ) return  2;
  if(stz=="BT"    ) return  3;
  if(stz=="ZP4"   ) return  4;
  if(stz=="ZP5"   ) return  5;
  if(stz=="ZP6"   ) return  6;
  if(stz=="ZP7"   ) return  7;
  if(stz=="WAST"  ) return  8;
  if(stz=="JST"   ) return  9;
  if(stz=="EAST"  ) return  10;
  if(stz=="UTC+11") return  11;
  if(stz=="IDLE"  ) return  12;
  if(stz=="IDLW"  ) return -12;
  if(stz=="NT"    ) return -11;
  if(stz=="AHST"  ) return -10;
  if(stz=="YST"   ) return -9;
  if(stz=="PST"   ) return -8;
  if(stz=="MST"   ) return -7;
  if(stz=="CST"   ) return -6;
  if(stz=="EST"   ) return -5;
  if(stz=="AST"   ) return -4;
  if(stz=="UTC-3" ) return -3;
  if(stz=="AT"    ) return -2;
  if(stz=="WAT"   ) return -1;
  return 0;
}

std::string
miutil::miTime::format(const std::string& newTime, const miDate::lang l) const
{
  return format(newTime, (l== miDate::Norwegian ? "no" : "en" ) );
}


std::string
miutil::miTime::format(const std::string& nt, const std::string& lang) const
{
  std::string newTime(nt), l(lang);
  miutil::replace(newTime, "%c","%a %b %d %X GMT %Y");

  miTime ftim(Date,Clock);

  int k;
  vector<std::string> token,remove;

  if(miutil::contains(newTime, "$")) {
    token = miutil::split(newTime);

    for(unsigned int i=0;i<token.size();i++) {
      if(miutil::contains(token[i], "$")) {

        if((k=token[i].find("$tz="))!=string::npos) {
	  token[i]= token[i].substr(k+4);
	  miutil::replace(newTime, "%tz", token[i]);
	  ftim.addHour( ftim.timezone(token[i]) );
	  remove.push_back("$tz=" + token[i]);
        }
        if(miutil::contains(token[i], "$dst")){
	  ftim.addHour(ftim.dst());
          remove.push_back("$dst");
        }
        if(miutil::contains(token[i], "$time")){
	  miutil::replace(newTime, token[i],"%Y-%m-%d %H:%M:%S");
        }
        if(miutil::contains(token[i], "$date")){
	  miutil::replace(newTime, token[i],"%Y-%m-%d");
        }
        if(miutil::contains(token[i], "$clock")){
	  miutil::replace(newTime, token[i],"%H:%M:%S");
        }
        if(miutil::contains(token[i], "$autoclock")){
          if(Clock.sec()!=0)
	    miutil::replace(newTime, token[i],"%H:%M:%S");
          else if(Clock.min()!=0)
	    miutil::replace(newTime, token[i],"%H:%M");
	  else
	    miutil::replace(newTime, token[i],"%H");
        }
        if(miutil::contains(token[i], "$miniclock")){
          if(Clock.min()!=0)
	    miutil::replace(newTime, token[i],"%H:%M");
	  else
	    miutil::replace(newTime, token[i],"%H");
        }
        if((k=token[i].find("$lg="))!=string::npos) {
	  token[i]= token[i].substr(k+4);
	  if(miutil::contains(token[i], "nor"))
	    l = "no";
	  else if(miutil::contains(token[i], "eng"))
	    l = "en";
	  else if(miutil::contains(token[i], "swe"))
	    l = "se";
	  else
	    l=token[i];
	  remove.push_back("$lg=" + token[i]);
        }

        if (remove.size()) {
          for (unsigned int n=0; n<remove.size(); n++) {
	    std::string rm1= " " + remove[n];
	    std::string rm2= remove[n] + " ";
	    if (miutil::contains(newTime, rm1))
	      miutil::replace(newTime, rm1, "");
	    else if (miutil::contains(newTime, rm2))
	      miutil::replace(newTime, rm2, "");
	    else
	      miutil::replace(newTime, remove[n], "");
	  }
	  remove.clear();
        }
      }
    }
  }

  if (miutil::contains(newTime, "$midnight24")){
    if (ftim.clock().isoClock() == "00:00:00"){
      ftim.addDay(-1);
    } else {
      miutil::replace(newTime, " $midnight24", "");
      miutil::replace(newTime, "$midnight24 ", "");
      miutil::replace(newTime, "$midnight24", "");
    }
  }

  newTime = ftim.date().format(newTime,l);
  newTime = ftim.clock().format(newTime);
  return newTime;
}

std::string miutil::miTime::format(const miutil::miTime& time, const std::string& format)
{
	if (time.undef()){
		warning("format: Object is not initialised.");
		return format;
	}
	
	time_facet tf(format.c_str());
	stringstream log_stream;

	struct tm t;
	t.tm_sec=time.sec();
	t.tm_min=time.min();
	t.tm_hour=time.hour();
	t.tm_mday=time.day();
	t.tm_mon=time.month() - 1;
	t.tm_year=time.year() - 1900;  
	t.tm_wday=time.dayOfWeek();
	t.tm_yday=time.dayOfYear();
	t.tm_isdst=0;  

	ptime _time = ptime_from_tm(t);
	tf.put(log_stream, log_stream,' ', _time);
	return log_stream.str();
}
