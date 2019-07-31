/*
  libpuTools - Basic types/algorithms/containers

  Copyright (C) 2006-2019 met.no

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


// miDate.cc -- functions for class miDate

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "miDate.h"

#include "miString.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cstdio>

#include <time.h>

using namespace std;
using namespace miutil;

miDate::Translations::~Translations()
{
}

// ------------------------------------------------------------------------

class miDateEnglish : public miDate::Translations {
public:
  const std::string& weekday(int day, bool) const override
    { return days[day]; }
  const std::string& shortweekday(int day, bool) const override
    { return daysShort[day]; }
  const std::string& monthname(int month, bool) const override
    { return months[month]; }
  const std::string& shortmonthname(int month, bool) const override
    { return monthsShort[month]; }
  static const std::string days[7], daysShort[7], months[12], monthsShort[12];
};

const std::string miDateEnglish::days[7] =
  { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
const std::string miDateEnglish::daysShort[7] =
  { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

const std::string miDateEnglish::months[12] =
  { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
const std::string miDateEnglish::monthsShort[12] =
  { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

// ------------------------------------------------------------------------

class miDateNorwegianNB : public miDate::Translations {
public:
  const std::string& weekday(int day, bool utf8) const override
    { return (utf8 ? daysU8 : daysL1)[day]; }
  const std::string& shortweekday(int day, bool utf8) const override
    { return (utf8 ? daysU8Short : daysL1Short)[day]; }
  const std::string& monthname(int month, bool) const override
    { return months[month]; }
  const std::string& shortmonthname(int month, bool) const override
    { return monthsShort[month]; }
  static const std::string daysU8[7], daysL1[7], daysU8Short[7], daysL1Short[7], months[12], monthsShort[12];
};

const std::string miDateNorwegianNB::daysU8[7] =
  { "Søndag" /* oslash */, "Mandag", "Tirsdag", "Onsdag", "Torsdag", "Fredag", "Lørdag" /* oslash */ };
const std::string miDateNorwegianNB::daysL1[7] =
  { "S\370ndag" /* oslash */, "Mandag", "Tirsdag", "Onsdag", "Torsdag", "Fredag", "L\370rdag" /* oslash */ };
const std::string miDateNorwegianNB::daysU8Short[7] =
  { "Søn", "Man", "Tir", "Ons", "Tor", "Fre", "Lør" };
const std::string miDateNorwegianNB::daysL1Short[7] =
  { "S\370n", "Man", "Tir", "Ons", "Tor", "Fre", "L\370r" };

const std::string miDateNorwegianNB::months[12] =
  { "Januar", "Februar", "Mars", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Desember" };
const std::string miDateNorwegianNB::monthsShort[12] =
  { "Jan", "Feb", "Mar", "Apr", "Mai", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Des" };

// ------------------------------------------------------------------------

class miDateNorwegianNN : public miDateNorwegianNB {
public:
  const std::string& weekday(int day, bool utf8) const override
    { return (utf8 ? daysU8 : daysL1)[day]; }
  const std::string& shortweekday(int day, bool utf8) const override
    { return (utf8 ? daysU8Short : daysL1Short)[day]; }
  static const std::string daysU8[7], daysL1[7], daysU8Short[7], daysL1Short[7];
};

const std::string miDateNorwegianNN::daysU8[7] =
  { "Søndag", "Måndag", "Tysdag", "Onsdag", "Torsdag", "Fredag", "Laurdag" };
const std::string miDateNorwegianNN::daysL1[7] =
  { "S\370ndag", "M\345ndag", "Tysdag", "Onsdag", "Torsdag", "Fredag", "Laurdag" };

const std::string miDateNorwegianNN::daysU8Short[7] =
  { "Søn", "Mån", "Tys", "Ons", "Tor", "Fre", "Lau" };
const std::string miDateNorwegianNN::daysL1Short[7] =
  { "S\370n", "M\345n" /* aring */, "Tys", "Ons", "Tor", "Fre", "Lau" };

// ------------------------------------------------------------------------

class miDateGerman : public miDate::Translations {
public:
  const std::string& weekday(int day, bool) const override
    { return days[day]; }
  const std::string& shortweekday(int day, bool) const override
    { return daysShort[day]; }
  const std::string& monthname(int month, bool utf8) const override
  { return (utf8 ? monthsU8 : monthsL1)[month]; }
  const std::string& shortmonthname(int month, bool utf8) const override
    { return (utf8 ? monthsU8Short : monthsL1Short)[month]; }
  static const std::string days[7], daysShort[7], monthsL1[12], monthsU8[12], monthsL1Short[12], monthsU8Short[12];
};

const std::string miDateGerman::days[7] =
  { "Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag" };
const std::string miDateGerman::daysShort[7] =
  { "So", "Mo", "Di", "Mi", "Do", "Fr", "Sa"};

const std::string miDateGerman::monthsL1[12] =
  { "Jan", "Feb", "M\344r", "Apr", "Mai", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Dez" };
const std::string miDateGerman::monthsU8[12] =
  { "Jan", "Feb", "Mär", "Apr", "Mai", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Dez" };
const std::string miDateGerman::monthsL1Short[12] =
  { "Januar", "Februar", "M\344rz", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember" };
const std::string miDateGerman::monthsU8Short[12] =
  { "Januar", "Februar", "März", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember" };

// ------------------------------------------------------------------------

class miDateSwedish : public miDate::Translations {
public:
  const std::string& weekday(int day, bool utf8) const override
    { return (utf8 ? daysU8 : daysL1)[day]; }
  const std::string& shortweekday(int day, bool utf8) const override
    { return (utf8 ? daysU8Short : daysL1Short)[day]; }
  const std::string& monthname(int month, bool) const override
    { return months[month]; }
  const std::string& shortmonthname(int month, bool) const override
    { return monthsShort[month]; }
  static const std::string daysU8[7], daysL1[7], daysU8Short[7], daysL1Short[7], months[12], monthsShort[12];
};

const std::string miDateSwedish::daysU8[7] =
  { "Söndag", "Måndag", "Tisdag", "Onsdag", "Torsdag", "Fredag", "Lördag" };
const std::string miDateSwedish::daysL1[7] =
  { "S\366ndag", "M\345ndag", "Tisdag", "Onsdag", "Torsdag", "Fredag", "L\366rdag" };
const std::string miDateSwedish::daysU8Short[7] =
  { "Sön",  "Mån", "Tis", "Ons", "Tor", "Fre", "Lör" };
const std::string miDateSwedish::daysL1Short[7] =
  { "S\366n", "M\345n", "Tis", "Ons", "Tor", "Fre", "L\366r" };

const std::string miDateSwedish::months[12] =
  { "Januari", "Februari", "Mars", "April", "Maj", "Juni", "Juli", "Augusti", "September", "Oktober", "November", "December" };
const std::string miDateSwedish::monthsShort[12] =
  { "Jan", "Feb", "Mar", "Apr", "Maj", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Dec" };


// ------------------------------------------------------------------------

// static
miDate::translations_t miDate::translations;

// static
void miDate::initTranslations()
{
  if (translations.empty()) {
    translations["nb"] = translations["no"] = std::make_shared<miDateNorwegianNB>();
    translations["nn"] = std::make_shared<miDateNorwegianNN>();
    translations["de"] = std::make_shared<miDateGerman>();
    translations["se"] = translations["sv"] = std::make_shared<miDateSwedish>();
    translations["en"] = std::make_shared<miDateEnglish>();
  }
  if (!defaultLanguage) {
    translations_t::const_iterator it = translations.find("en");
    if (it == translations.end())
      it = translations.begin();
    defaultLanguage = it->second;
  }
}

miDate::Translations_cp miutil::miDate::defaultLanguage;

// static
void miDate::installTranslation(miDate::Translations_cp t, const std::string& languagecode)
{
  if (t) {
    translations[languagecode] = t;
    defaultLanguage = t;
  }
}

// static
void miDate::setDefaultLanguage(const std::string& l)
{
  defaultLanguage = language(l);
}

// static
miDate::Translations_cp miDate::language(const std::string& l)
{
  initTranslations();

  if (!l.empty()) {
    translations_t::const_iterator it = translations.find(l);
    if (it == translations.end())
      it = translations.find(miutil::to_lower(l));
    if (it != translations.end())
      return it->second;
  }
  return defaultLanguage;
}

// ########################################################################

static inline void warning(const std::string& s)
{
  std::cerr << "Warning: miDate::" << s << std::endl;
}

static const long julianDayZero=1721425;

static const int monthLength[14]={
   0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 0 };

static const int cum_ml[2][16]={
  { 0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365, 400, 0 },
  { 0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366, 400, 0 }};

static inline long lfloor(const long a, const long b) // assumes b positive
{ return a>=0? a/b: (a%b==0)-1-labs(a)/b; }

static inline int isLeap(const int y)
{ return ((y%4==0 && y%100!=0) || y%400==0); }

static bool scan_date(const std::string& str, int& y, int& m, int& d)
{
  int n = -1;
  if (str.size() == 4 + 1 + 2 + 1 + 2)
    n = sscanf(str.c_str(), "%4d-%2d-%2d", &y, &m, &d);
  else if (str.size() == 4 + 2 + 2)
    n = sscanf(str.c_str(), "%4d%2d%2d", &y, &m, &d);
  if (n == 3)
    return true;
  y = m = d = 0;
  return false;
}

int
miutil::miDate::daysInYear() const
{ return 365+isLeap(Year); }

int
miutil::miDate::daysInMonth() const
{ return monthLength[Month]+(Month==2 && isLeap(Year)); }

int
miutil::miDate::dayOfYear() const
{ return cum_ml[isLeap(Year)][Month]+Day; }

void
miutil::miDate::setDate(int y, int m, int d)
{
  if(!isValid(y,m,d) ) {
#if 0 // this is called from the default ctor with y=m=d=0
    std::ostringstream w;
    w << "setDate: Illegal date! YYYY-MM-DD (" << y << '-' << m << '-' << d << ')';
    warning(w.str());
#endif
    Year=Month=Day=jdn=0;
    return;
  }

  Year  = y;
  Month = m;
  Day   = d;

  jdn=(Year-1)*365 + lfloor(Year-1,4L) +
    lfloor(Year-1,400L) - lfloor(Year-1,100L) +
    cum_ml[isLeap(Year)][Month] + Day + julianDayZero;
}

void
miutil::miDate::setDate(const std::string& str)
{
  int y, m, d;
  scan_date(str, y, m, d);
  setDate(y,m,d);
}

bool
miutil::miDate::isValid(int y, int m, int d)
{
  if (m<=0 || m>12 || d<0 || d>31)
    return false;
  if ( d > ( monthLength[m]+(m==2 && isLeap(y))))
    return false;
  return true;
}

bool
miutil::miDate::isValid(const std::string& str)
{
  int y, m, d;
  if (!scan_date(str, y, m, d))
    return false;
  return isValid(y,m,d);
}

/*
 * Arithmetic
 */

void
miutil::miDate::addDay(const long add)
{
  if (undef()) {
    warning("addDay: Can't add days. Date is undefined.");
    return;
  }

  jdn+=add;
  jdntodate(jdn);
}

/*
 * Conversion functions
 */

miutil::miDate&
miutil::miDate::jdntodate(long dn) // Convert Julian day number (dn) to date
{
  if (undef()) {
    warning("jdntodate: Date is undefined.");
    return *this;
  }

  int exception=0;

  const long y400=146097; // == 400*365.2425
  const long y100=36524;  // == round(100*365.2425)
  const long y4=1461;     // == round(4*365.2425)

  jdn=dn;
  dn-=julianDayZero+1;

  Year=400*lfloor(dn,y400);    // 400-year periods
  dn-=y400*lfloor(dn,y400);

  if (dn>0) {
    Year+=100*lfloor(dn,y100); // 100-year periods
    dn-=y100*lfloor(dn,y100);
    exception=(dn==0);         // Occurs once a century
    if (dn>0) {
      Year+=4*lfloor(dn,y4);   // 4-year periods
      dn-=y4*lfloor(dn,y4);
      if (dn>0) {
	int i=0;
	while (dn>365 && ++i<4) {
	  Year++;
	  dn-=365;
	}
      }
    }
  }

  if (exception)
    dn=366;
  else {
    Year++;
    dn++;
  }

  Month=1;
  while (cum_ml[isLeap(Year)][Month]<dn)
    Month++;
  Month--;
  dn-=cum_ml[isLeap(Year)][Month];
  if (Month==13) {
    Month=1;
    Year++;
  }

  Day=dn;

  return *this;
}

// Return a string with date formatted according to ISO
// standards (ISO 8601)
std::string
miutil::miDate::isoDate() const
{
  if (undef())
    warning("isoDate: Date is undefined.");

  ostringstream ost;

  ost << setw(4) << setfill('0') << Year  << '-'
      << setw(2) << setfill('0') << Month << '-'
      << setw(2) << setfill('0') << Day;

  return ost.str();
}

// Returns the week number. Week 1 of a year is per definition the
// first week that contains a Thursday.
int
miutil::miDate::weekNo() const
{
  if (undef()) {
    warning("weekNo: Date is undefined. Can't compute week number.");
    return 0;
  }

  miDate mon(Year,1,1);
  int monD=(mon.intWeekday()+6)%7;

  if (monD<4)
    mon.addDay(-monD);
  else
    mon.addDay(7-monD);

  return (jdn-mon.jdn)/7+1;
}

// static
const std::string& miDate::languagestring(miDate::lang l)
{
  static const std::string LANG[2] = { "en", "no" }; // must match "lang" enum
  return LANG[l];
}


std::string
miutil::miDate::weekday(const lang l) const
{
  return weekday(languagestring(l));
}


std::string
miutil::miDate::weekday(const std::string& l) const
{
  return weekday(l, false);
}

std::string
miutil::miDate::weekday(const std::string& l, bool utf8) const
{
  if (undef()) {
    warning("weekday: Date is undefined. Can't find weekday.");
    return "";
  }

  return language(l)->weekday(intWeekday(), utf8);
}

std::string
miutil::miDate::shortweekday(const lang l) const
{
  return shortweekday(languagestring(l));
}

std::string
miutil::miDate::shortweekday(const std::string& l) const
{
  return shortweekday(l, false);
}

std::string
miutil::miDate::shortweekday(const std::string& l, bool utf8) const
{
  if (undef()) {
    warning("shortWeekday: Date is undefined. Can't find weekday.");
    return "";
  }

  return language(l)->shortweekday(intWeekday(), utf8);
}


std::string
miutil::miDate::monthname(const lang l) const
{
  return monthname(languagestring(l));
}

std::string
miutil::miDate::monthname(const std::string& l) const
{
  return monthname(l, false);
}

std::string
miutil::miDate::monthname(const std::string& l, bool utf8) const
{
  if (undef()) {
    warning("monthname: Date is undefined. Can't return month name.");
    return "";
  }

  return language(l)->monthname(Month-1, utf8);
}


std::string
miutil::miDate::shortmonthname(const lang l) const
{
  return shortmonthname(languagestring(l));
}

std::string
miutil::miDate::shortmonthname(const std::string& l) const
{
  return shortmonthname(l, false);
}

std::string
miutil::miDate::shortmonthname(const std::string& l, bool utf8) const
{
  if (undef()) {
    warning("monthShortname: Date is undefined. Can't return month name.");
    return "";
  }

  return language(l)->shortmonthname(Month-1, utf8);
}


miutil::miDate
miutil::miDate::easterSundayThisYear() const
{
  if (undef()) {
    warning("easterSundayThisYear: Date is undefined. Can't find easter.");
    return *this;
  }

  int g=Year%19;
  int c=Year/100;
  int h=(c - c/4 - (8*c+13)/25 + 19*g + 15)%30;
  int i=h - (h/28)*(1 - (h/28)*(29/(h+1))*((21-g)/11));
  int j=(Year + Year/4 + i + 2 - c + c/4)%7;
  int l=i-j;

  int eMonth=3+(l+40)/44;
  int eDay  =l+28-31*(eMonth/4);

  miDate easterSunday(Year,eMonth,eDay);

  return easterSunday;
}

miutil::miDate
miutil::miDate::today()
{
  time_t tp=time(0);
  struct tm *ts=gmtime(&tp);
  int y=ts->tm_year+1900;
  int m=ts->tm_mon+1;
  int d=ts->tm_mday;

  return miDate(y,m,d);
}


std::string
miutil::miDate::format(const std::string& newDate,const lang l) const
{
  return format(newDate, languagestring(l));
}

std::string
miutil::miDate::format(const std::string& newDate, const std::string& l) const
{
  return format(newDate, l, false);
}

std::string
miutil::miDate::format(const std::string& newDate, const std::string& l, bool utf8) const
{
  if(undef())
    return newDate;

  std::string d(newDate);
  miutil::replace(d, "%y", miutil::from_number(Year%100, 2)); //!%y  last two digits of year (00..99)
  miutil::replace(d, "%Y", miutil::from_number(Year, 4));     //!%Y  year (1970...)

  miutil::replace(d, "%d", miutil::from_number(Day,2));      //!%d  day of month (01..31)
  miutil::replace(d, "%e", miutil::from_number(Day));        //!%e  day of month ( 1..31)
  miutil::replace(d, "%m", miutil::from_number(Month,2));    //!%m  month (01..12)

  miutil::replace(d, "%D", isoDate());            //!%D  date (yyyy-mm-dd)

  miutil::replace(d, "%B", monthname(l, utf8));         //!%B  month  name,  (January..December)
  miutil::replace(d, "%b", shortmonthname(l, utf8));    //!%b  short month  name,  (Jan..Dec)
  miutil::replace(d, "%A", weekday(l, utf8));           //!%A  weekday name, (Sunday..Saturday)
  miutil::replace(d, "%a", shortweekday(l, utf8));      //!%a  shortweekday name, (Sun..Sat)

  miutil::replace(d, "%V", miutil::from_number(weekNo())); //!%V  week number

  miutil::replace(d, "%_B", miutil::to_lower(monthname(l, utf8)));      //!%B  month  name, lowercase
  miutil::replace(d, "%_b", miutil::to_lower(shortmonthname(l, utf8))); //!%b  short month  name, lowercase
  miutil::replace(d, "%_A", miutil::to_lower(weekday(l, utf8)));        //!%A  weekday name,lowercase
  miutil::replace(d, "%_a", miutil::to_lower(shortweekday(l, utf8)));   //!%a  shortweekday name,lowercase

  return d;
}
