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

#ifndef _puMathAlgo_h
#define _puMathAlgo_h

#include <iterator>

#include <cstdlib>
#include <cmath>
#include <numeric>

namespace puMathAlgo {

// Randomiser
class randomInt {
public:
  unsigned long operator()(unsigned long max);
  void seed(long s);
};


template<class C>
typename C::value_type percentile(const C& seq_sorted, const double p)
{
  double np = p*(seq_sorted.size()-1);
  typename C::const_iterator low = seq_sorted.begin(), high = seq_sorted.begin();
  std::advance(low,  static_cast<int>(std::floor(np)));
  std::advance(high, static_cast<int>(std::ceil(np)));
  return (*high + *low)/2;
}


template<class C, class sorter>
typename C::value_type percentile(const C& seq, const double p, sorter& sf)
{
  C sseq(seq);
  sf(sseq);
  return percentile(sseq, p);
}


template<class C, class sorter>
inline typename C::value_type median(const C& seq, sorter& sf)
{
  return percentile(seq, 0.5, sf);
}


template<class C>
inline typename C::value_type median(const C& seq_sorted)
{
  return percentile(seq_sorted, 0.5);
}


template<class C>
typename C::value_type mean(const C& seq)
{
  typename C::value_type zero;
  if (seq.empty())
    return zero;
  else
    return std::accumulate(seq.begin(), seq.end(), zero) / seq.size();
}


// class to sum and average values

template<class T>
class average {
private:
  T sum;
  size_t iter;
  T DUMMY;

public:
  average(const T& dum)
    : sum(), iter(0), DUMMY(dum) {}

  T operator()() const {
    return ( iter > 0 ? sum/iter : DUMMY );
  }

  void add(const T& s) {
    if (s != DUMMY) {
      sum += s;
      iter++;
    }
  }

  void clear() {
    sum = T();
    iter = 0;
  };

  T middle() {
    T av = (*this)();
    clear();
    return (av);
  };

  friend average<T> operator+(const average<T>& lhs, const average<T>& rhs){
    average<T> res(lhs.DUMMY);
    res.sum = lhs.sum+rhs.sum;
    res.iter = lhs.iter+rhs.iter;
    return res;
  }

  average<T>& operator=(const average<T>& rhs) {
    sum = rhs.sum;
    iter = rhs.iter;
    DUMMY = rhs.DUMMY;
  }
};

} // namespace puMathAlgo

#endif
