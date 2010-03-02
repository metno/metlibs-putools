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


// miString.cc : definitions for miString class
// Authors: ØA/LBS March 1997

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>
#include <ctype.h>
#include <iostream>
#include <iomanip>
#include <strstream>
#include "miString.h"

int miString::istrMax=200;
int miString::numMax=20;

// construct miString object from char array
miString::miString(const char* t)
{
  if (t) {
    data = new char[strlen(t)+1];
    strcpy(data,t);
  }
  else {
    data = new char[1];
    *data='\0';
  }
}

// copy constructor
miString::miString(const miString& rhs)
{
  data = new char[strlen(rhs.data)+1];
  strcpy(data,rhs.data);
}

// Create string from floats
miString::miString(const double f)
{
  char* c = new char[numMax];
  ostrstream cs(c,numMax);

  cs << f << ends;
  data = new char[strlen(c)+1];
  strcpy(data,c);
  delete[] c;
}

// and from integers
miString::miString(const int i, const int width, const char fill)
{
  char* c = new char[numMax];
  ostrstream cs(c,numMax);
  
  if (width>0)
    cs << setw(width) << setfill(fill) << i << ends;
  else
    cs << i << ends;

  data = new char[strlen(c)+1];
  strcpy(data,c);
  delete[] c;
}

miString& miString::operator=(const char* rhs)  // assignment operator
{
  delete [] data;
  if (rhs) {
    data = new char[strlen(rhs)+1];
    strcpy(data,rhs);
  }
  else {
    data = new char[1];
    *data='\0';
  }
  return *this;
}

miString& miString::operator=(const miString& rhs) // assignment operator
{
  if (this != &rhs) {                     // protect against 's = s'
    delete [] data;                       // delete old miString
    data = new char[strlen(rhs.data)+1];
    strcpy(data,rhs.data);
  }
  return *this;
}

miString operator+(const miString& lhs, const miString& rhs) // concatenation
{
  miString s;
  s.data=new char[strlen(lhs.data)+strlen(rhs.data)+1];
  strcat(strcpy(s.data,lhs.data),rhs.data);

  return s;
}

miString& miString::operator+=(const miString& rhs) // assignment +=
{
  if (rhs.length()>0) {
    char* tmp = data;
    if (this == &rhs) { // take care if 'a+=a'
      data = new char[2*strlen(data)+1];
      strcat(strcpy(data,tmp),tmp);
    }
    else {
      data = new char[strlen(data)+strlen(rhs.data)+1];
      strcat(strcpy(data,tmp),rhs.data);
    }
    delete[] tmp;
  }
  return *this;
}

istream& operator>>(istream& istr, miString& s)
{
  char* ch = new char[miString::istrMax+1];
  istr.getline(ch,miString::istrMax+1);
  s = ch;
  delete[] ch;
  return istr;
}

void miString::trim(bool l, bool r)
{
  if (r)
    for (int i=strlen(data)-1; isspace(*(data+i)) && i>=0; *(data+i--)='\0');
  if (l) {
    char *tmp1,*tmp2;
    for (tmp1=data; isspace(*tmp1); tmp1++);
    if (tmp1!=data) {
      tmp2 = new char[strlen(tmp1)+1];
      strcpy(tmp2,tmp1);
      delete[] data;
      data=tmp2;
    }
  }
}

int miString::countChar(const char c) const
{
  char* tmp;
  int n=0;

  for (tmp=data; *tmp; tmp++)
    if (*tmp==c)
      n++;

  return n;
}

void miString::remove(const char c)
{
  int n=countChar(c);

  if (n==0)
    return;

  char* newData = new char[strlen(data)-n+1];
  char  *tmp, *tmp2;

  for (tmp=data, tmp2=newData; *tmp; tmp++)
    if (*tmp!=c)
      *tmp2++=*tmp;
  *tmp2='\0';

  delete[] data;
  data=newData;
}


miString miString::replace(const char f, const char t) const
{
  miString s(*this);
  char* tmp;

  for (tmp=s.data; *tmp; tmp++)
    if (*tmp==f)
      *tmp=t;

  return s;
}

vector<miString> miString::split(const char c, const bool clean) const
{
  vector<miString> vec;
  char *tmp, *p, *first;

  first = new char[strlen(data)+1];
  strcpy(first,data);

  for (p=tmp=first; *p; p++)
    if (*p==c) {
      *p='\0';
      vec.push_back(tmp);
      p=tmp=++p;
    }
  vec.push_back(tmp);
  
  delete[] first;

  if (clean)
    for (int i=0; i<vec.size(); i++) {
      vec[i].trim();
      if (!(vec[i].exists()))
	vec.erase(vec.begin()+i--);
    }
    
  return vec;
}

miString miString::upcase(bool isoLatin1) const
{
  miString s(*this);

  for (char *tmp=s.data; *tmp=toupper(*tmp); tmp++)
    if (isoLatin1) { // Accented characters and the like
      if ((*tmp>=224 && *tmp<=246) || (*tmp>=248 && *tmp<=254))
	*tmp-=32;
      if (*tmp==223) { // The difficult eszet...
	char *t = new char[strlen(s.data)+2];
	char *tmp2;
	int i;
	for (tmp2=s.data, i=0; tmp2<tmp; tmp2++, i++)
	  t[i]=*tmp2;
	t[i++]='S';
	tmp=t+i;
	t[i++]='S';
	for (++tmp2; *tmp2; tmp2++, i++)
	  t[i]=*tmp2;
	t[i]='\0';
	delete[] s.data;
	s.data=t;
      }
    }
  return s;
}

miString miString::downcase(bool isoLatin1) const
{
  miString s(*this);

  for (char *tmp=s.data; *tmp=tolower(*tmp); tmp++)
    if (isoLatin1) // Accented characters and the like
      if ((*tmp>=192 && *tmp<=214) || (*tmp>=215 && *tmp<=222))
	*tmp+=32;
  return s;
}

bool miString::isNumber() const
{
  bool fl=false;
  char* tmp, *tmp2;

  for (tmp=data; isspace(*tmp); tmp++);
  for (tmp2=data+strlen(data)-1; isspace(*tmp2); tmp2--);

  for (; tmp<=tmp2; tmp++)
    if (!isdigit(*tmp))
      if (*tmp=='.')
	if (fl)
	  return false;
	else
	  fl=true;
      else
	return false;

  return true;
}
