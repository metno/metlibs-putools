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


#ifndef _minmax_h
#define _minmax_h

#include <vector>

using namespace std;

template<class T> T Max(T a, T b)
{
  return a>b? a: b;
}

template<class T> T Min(T a, T b)
{
  return a<b? a: b;
}

template<class T> void MinMax(const vector<T>& s, T& min, T& max)
{
  int nit=s.size();
  if (!nit) return;
  min=max=s[0];
  for (int i=0; i<nit; ++i) {
    if (s[i]<min)
      min=s[i];
    else if (s[i]>max)
      max=s[i];
  }
}
#endif
