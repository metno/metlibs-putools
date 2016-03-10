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

#ifndef _minmax_h
#define _minmax_h

#include <functional>

template<class It, class T, class Comp>
bool MinMax(It begin, It end, T& mini, T& maxi, const Comp& comp = Comp())
{
  if (begin == end)
    return false;

  mini = maxi = *begin;
  for (++begin; begin != end; ++begin) {
    if (comp(*begin, mini))
      mini = *begin;
    else if (comp(maxi, *begin))
      maxi = *begin;
  }
  return true;
}

template<class It, class T>
bool MinMax(It begin, It end, T& mini, T& maxi)
{
  return MinMax(begin, end, mini, maxi, std::less<T>());
}

template<class C, class Comp>
bool MinMax(const C& s, typename C::value_type& mini, typename C::value_type& maxi, const Comp& comp = Comp())
{
  return MinMax(s.begin(), s.end(), mini, maxi, comp);
}

template<class C>
bool MinMax(const C& s, typename C::value_type& mini, typename C::value_type& maxi)
{
  return MinMax<C, std::less<typename C::value_type> >(s, mini, maxi);
}

#endif // _minmax_h
