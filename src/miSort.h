/*
  libpuTools - Basic types/algorithms/containers

  Copyright (C) 2006-2016 met.no

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

#include <algorithm>

namespace miutil {

template<class T> void Qsort(T* a, int l, int r)
{
  if (l>=r)
    return;

  std::swap(a[l], a[(l+r)/2]);
  int z = l;
  for (int i=l+1; i<=r; i++)
    if (a[i] < a[l])
      std::swap(a[++z], a[i]);

  std::swap(a[l], a[z]);
  Qsort(a, l,z-1);
  Qsort(a, z+1,r);
}

template<class T>
bool Bsearch(const T& key, T* a, int n)
{
  return std::binary_search(a, a+n, key);
}

} // namespace miutil

#endif
