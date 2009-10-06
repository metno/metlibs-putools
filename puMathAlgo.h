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


#ifndef _puMathAlgo_h
#define _puMathAlgo_h

#include <stdlib.h>
#include <math.h>
#include <numeric>

namespace puMathAlgo {

  // Randomiser
class randomInt {
public:
  unsigned long operator()(unsigned long max) {
    unsigned long rval=random();
    return rval%max;
  }
  void seed(long s) { srandom(s); }
};

  // Statistical functions: percentile, mean, and median
  // Works for (at least) vectors, lists and sets of numerical types.
  // The return value is type double.
  // Percentile and median need a functor for sorting.

template<class C, class sorter>
double percentile(const C& seq, const double p, sorter& sf) {
  C sseq(seq);
  sf(sseq);
  int n=sseq.size();
  double np=p*(n-1);
  typename C::iterator low=sseq.begin(), high=sseq.begin();
  advance(low, static_cast<int>(floor(np)));
  advance(high, static_cast<int>(ceil(np)));
  return (*high+*low)/2;
}

template<class C, class sorter>
inline double median(const C& seq, sorter& sf) {
  return percentile(seq, 0.5, sf);
}

template<class C>
double mean(const C& seq) {
  double acc=0;
  int n=seq.size();
  acc=accumulate(seq.begin(), seq.end(), 0);
  return acc/n;
}

// class to sum and average values

template<class T> class average {
private:
  T sum;
  T iter;
  T DUMMY;
public:
  average(const T& dum )
    : sum(0),iter(0),DUMMY(dum) {}
  
  T operator()() const {
    return ( iter > 0 ? sum/iter : DUMMY );
  }
  
  void add(const T& s){
    if(s != DUMMY){
      sum += s;
      iter++;
    }
  };
  
  void clear(){
    sum = 0;
    iter = 0;
  };
   
  T middle(){
    T av =( iter > 0 ? sum/iter : DUMMY );
    clear();
  return(av);
  };

  friend average<T> operator+( const average<T>& lhs, const average<T>& rhs){
    average<T> res(lhs.DUMMY);
    res.sum = lhs.sum+rhs.sum;
    res.iter = lhs.iter+rhs.iter;
    return res;
  }

  average<T>& operator=(const average<T>& rhs){
    sum = rhs.sum;
    iter = rhs.iter;
    DUMMY = rhs.DUMMY;
  };
};
}
#endif
