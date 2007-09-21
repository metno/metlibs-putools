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


#ifndef _puAlgo_h
#define _puAlgo_h

#include <algorithm>

namespace puAlgo {

  // The idle functor -- accept an argument and do nothing
class do_nothing {
public:
  explicit do_nothing() {}
  template<class C> inline void operator()(C&) {}
};

  // Extensions of standard for algorithms
template<class C, class Op>
inline Op for_each_in(const C& c, Op f)
{
  return for_each(c.begin(), c.end(), f);
}

template<class C, class Op>
inline Op for_each_in(C& c, Op f)
{
  return for_each(c.begin(), c.end(), f);
}

template <class In, class Pred, class Op>
Op for_each_if(In first, In last, Pred p, Op f)
{
  for ( ; first != last; ++first)
    if (p(*first))
      f(*first);
  return f;
}

// Algorithms (functors) for sorting various containers
class sort_normal_ {
public:
  explicit sort_normal_() {}
  template<class C> void operator()(C& v) {
    sort(v.begin(), v.end());
  }
};

class sort_list {
public:
  explicit sort_list() {}
  template<class C> void operator()(C& l) {
    l.sort();
  }
};

typedef sort_normal_ sort_vector;
typedef do_nothing sort_set;

}
#endif



