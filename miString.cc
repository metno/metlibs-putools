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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "miString.h"


using namespace puAlgo;
using namespace std;



string miutil::miString::whitespaces(" \r\t\n");



miutil::miString::miString(const char* s)
{
  *this = (s ? std::string(s) : std::string(""));
}





miutil::miString::miString(const int i, const int width, const char fill)
  : string()
{
  ostringstream ost;
  if (width>0){
#if defined(__WIN32__) || defined(__WIN64__)
    ost << setw(width) << setfill(static_cast<char>(fill)) << i;
#else
    ost << setw(width) << setfill(fill) << i;
#endif
  } else
    ost << i;

  *this=ost.str();
}

miutil::miString::miString(const double d, const int prec)
  : string()
{
  ostringstream ost;
  if (prec!=-1)
    ost.precision(prec);
  ost << d;
  *this=ost.str();
}

miutil::miString::miString(const float d, const int prec)
  : string()
{
  ostringstream ost;
  if (prec!=-1)
    ost.precision(prec);
  ost << d;
  *this=ost.str();
}



int
miutil::miString::toInt( const int undefined ) const
{
  if(empty())
    return undefined;
  if(!isNumber())
    return undefined;
  return atoi(c_str());
}

long
miutil::miString::toLong( const long undefined ) const
{
  if(empty())
    return undefined;
  if(!isNumber())
    return undefined;
  return atol(c_str());
}

float
miutil::miString::toFloat( const float undefined ) const
{
  if(empty())
    return undefined;
  if(!isNumber())
    return undefined;
  return atof(c_str());
}

double miutil::miString::toDouble( const double undefined ) const
{
 if(empty())
   return undefined;
 if(!isNumber())
   return undefined;
 return atof(c_str());
}

void
miutil::miString::trim(bool l, bool r, const miString wspace)
{
  int len;
  int pos;

  if (empty())
    return;

  len=length();
  if (l) {
    pos=find_first_not_of(wspace);
    if (pos==npos) {
      erase();
      return;
    }
    if (pos>0 && pos<len)
      *this=substr(pos,len-pos);
  }
  if (r) {
    pos=find_last_not_of(wspace);
    if (pos==npos) {
      erase();
      return;
    }
    if (pos>=0 && pos<len-1)
      *this=substr(0,pos+1);
  }
}

void
miutil::miString::remove(const char c)
{
  if (empty())
    return;

  for (int pos=find(c); pos!=npos; pos=find(c))
    erase(pos,1);
}

miutil::miString
miutil::miString::replace(const char c1, const char c2) const
{
  miString s(*this);
  if (!empty()) {
    int len=length();
    for (int i=0; i<len; i++)
      if (s[i]==c1)
	s[i]=c2;
  }
  return s;
}

void
miutil::miString::replace(const miString& s1, const miString& s2)
{
  if (empty())
    return;
  int pos=0;
  int len1=s1.length();
  int len2=s2.length();
  for (pos=find(s1,pos); pos!=npos; pos=find(s1,pos)) {
    string::replace(pos,len1,s2);
    pos+=len2;
  }
}

vector<miutil::miString>
miutil::miString::split(const miString s, const bool clean) const
{
  return split(0,s,clean);
}

vector<miutil::miString>
miutil::miString::split(const char c, const bool clean) const
{
  return split(0,c, clean);
}


vector<miutil::miString>
miutil::miString::split(int nos, const char c, const bool clean) const
{
  char cs[2];
  cs[0]=c;
  cs[1]='\0';
  miString s(cs);
  return split(nos,cs, clean);
}

vector<miutil::miString>
miutil::miString::split(int nos,const miString s, const bool clean) const
{
  int splitnumber = 0;
  vector<miString> vec;
  if (!empty()) {
    int len=length();
    int start=(clean? find_first_not_of(s,0): 0);
    while (start>=0 && start<len) {
      int stop=find_first_of(s,start);
      if (stop<0 || stop>len)
	stop=len;
      vec.push_back(substr(start,stop-start));

      if(nos)
	if(++splitnumber >= nos) {
	  stop++;
	  if(stop < len )
	    vec.push_back(substr(stop,len-stop));
	  break;
	}
      start=(clean? find_first_not_of(s,stop+1): stop+1);
    }

    if (clean)
      for (int i=0; i<vec.size(); i++) {
	vec[i].trim();
	if (!(vec[i].exists()))
	  vec.erase(vec.begin()+i--);
      }
  }
  return vec;
}

// split with protected entities (i.e. () "" etc...



vector<miutil::miString>
miutil::miString::split(const char lb, // left border
			const char rb, // right border
			const miString s,
			const bool clean) const
{
  vector<miString> vec;
  if (!empty()) {

    int len=length();

    int start=(clean ? find_first_not_of(s,0): 0);

    while (start>=0 && start<len) {
      int stop;

      stop=find_first_of(s,start);

      int  tmp= start;
      bool isok=false;
      while ( !isok ){
	int lbp = find(lb,tmp);
	if ( lbp >= 0 && lbp < stop ){
	  int rbp = find(rb,lbp+1);
	  tmp=rbp+1;
	  if ( rbp >= 0 && rbp > stop ){
	    stop = find_first_of(s,tmp);
	  }
	} else isok=true;
      }

      if (stop<0 || stop>len)
	stop=len;

      vec.push_back(substr(start,stop-start));

      start=(clean? find_first_not_of(s,stop+1): stop+1);
    }


    if (clean)
      for (int i=0; i<vec.size(); i++) {
	vec[i].trim();
	if (!(vec[i].exists()))
	  vec.erase(vec.begin()+i--);
      }
  }
  return vec;
}




/**
 * Does this miString represent a number? Leading and trailing space
 * is allowed. A number consists of an optional + or - followed
 * by a decimal number. A decimal number consists of a non empty
 * sequence of digits and possibly a decimal point (.). An optional exponent
 * (e, E) followed by an optional + or -, followed by a non empty sequence
 * of decimal digits.
 *
 * ex. of numbers.
 *       .23  0.23 +0.23 -0.23 -0.23e-10
 *
 * ex. These strings are not considered as numbers.
 *     0.23e
 *     0.23 e+23 it is not allowed with a space before e.
 *     e+23
 *
 * BUGS:
 *   The decimalpoint shold be decided from the active local setting.
 *
 * NOTE:
 *   scanf doesn't accept as numbers strings that begins with a decimal
 *   point. Is this a bug in Linux libc or is it conforming to the C standard?
 *
 *       Ex. The string a=".23" give wrong result with sscanf.
 *           sscanf(a, "%f", &f)
 *
 * \return true if this string is a number false otherwise.
 * \see atof(3), atoi(3), strtod(3), scanf(3), printf(3)
 */

bool
miutil::miString::isNumber() const
{
  if (empty())
    return false;

  bool fl=false;
  string::const_iterator i1, i2;

  for (i1=begin(); i1!=end() && isspace(*i1); i1++);

  if(i1==end())
    return false;

  for (i2=end()-1; isspace(*i2); i2--);

  if(i2!=end())
    i2++;

  if(*i1=='-' || *i1=='+'){
    i1++;

    if(*i1=='e' || *i1=='E')
      return false;
    // only a - or + is not accepted
    if(i1==i2)
      return false;
  }

  if(*i1=='e' || *i1=='E')
    return false;

  for (; i1!=i2; i1++){
    if (!isdigit(*i1)){
      if (*i1=='.'){
	if (fl)
	  return false;
	else
	  fl=true;
      }else
	break;
    }
  }

  if(*i1=='e' || *i1=='E'){
    i1++;

    // we dont accept a number ending in e or E.
    if(i1==i2)
      return false;
  }

  if(*i1=='-' || *i1=='+'){
    i1++;

    if(i1==i2)
      return false;
  }

  for (; i1!=i2; i1++)
    if (!isdigit(*i1))
      return false;

 return true;
}

/**
 * Does this miString represent an int? Leading and trailing space
 * is allowed. A number consists of an optional + or - followed
 * by a non empty sequence of digits.
 *
 * ex. of ints.
 *       0 -1 +125
 *
 * \return true if this string is an int false otherwise.
 */

bool
miutil::miString::isInt() const
{
  if (empty())
    return false;

  string::const_iterator i1, i2;

  for (i1=begin(); i1!=end() && isspace(*i1); i1++);

  if(i1==end())
    return false;

  for (i2=end()-1; isspace(*i2); i2--);

  if(i2!=end())
    i2++;

  if(*i1=='-' || *i1=='+'){
    i1++;

    // only a - or + is not accepted
    if(i1==i2)
      return false;
  }

  for (; i1!=i2; i1++){
    if (!isdigit(*i1)){
      return false;
    }
  }

 return true;
}

miutil::miString
miutil::miString::upcase(int start, int len) const
{
  if( start < 0 ) start = 0;
  if( len   < 0 ) len   = 0;

  miString s(*this);
  int c, n=s.length(),stop=start+len;

  if( start > n ) return s;
  if( !len      ) stop = n;
  if( stop > n  ) stop = n;

  for (int i=start; i<stop; i++) {
    c=toupper(s[i]);
    if ((c>=224 && c<=246) || (c>=248 && c<=254))
      c-=32;
    s[i] = c;
  }
  return s;
}

miutil::miString
miutil::miString::downcase(int start, int len) const
{
  if( start < 0 ) start = 0;
  if( len   < 0 ) len   = 0;

  miString s(*this);
  int c, n=s.length(),stop=start+len;

  if( start > n ) return s;
  if( !len      ) stop = n;
  if( stop > n  ) stop = n;


  for (int i=start; i<stop; i++) {
    c=tolower(s[i]);
    if ((c>=192 && c<=214) || (c>=215 && c<=222))
      c+=32;
    s[i] = c;
  }
  return s;
}
