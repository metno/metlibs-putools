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


/* miSort.h:
   Simple template functions for sorting and searching arrays of arbitrary class.
   operator::< and > are assumed to exist for the class.
   Author: LBS, 20 Apr 1998
*/

#ifndef _miSort_h
#define _miSort_h


template<class T> void Swap(T& x, T& y)
{
  T tmp = x;
  x = y;
  y = tmp;
}

template<class T> void Qsort(T* a, int l, int r)
{
  int i, z;

  if (l>=r)
    return;

  Swap(a[l], a[(l+r)/2]);
  z=l;
  for (i=l+1; i<=r; i++)
    if (a[i]<a[l])
      Swap(a[++z],a[i]);

  Swap(a[l],a[z]);
  Qsort(a,l,z-1);
  Qsort(a,z+1,r);
}

template<class T> bool Bsearch(T& key, T* a, int n)
{
  int lo,hi,mi;

  lo=0;
  hi=n-1;
  while (lo<=hi) {
    mi=(lo+hi)/2;
    if (key<a[mi])
      hi=mi-1;
    else if (key>a[mi])
      lo=mi+1;
    else
      return true;
  }
  return false;
}

#endif
