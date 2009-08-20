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

#ifndef _miRing_h
#define _miRing_h

// ----------------------------------
// Generic ring buffer
// ----------------------------------
// Author: ADC 1997
// 28.06-1999 Added []-operator, ADC
// ----------------------------------

template<class T> class ring {
  T *b; // the buffer
  int p; // index of latest pushed element in buffer
  int Size; // size of ringbuffer
  int Num; // number of elements in buffer

public:
  ring(const int size = 20) // constructor takes size of buffer
  :
    p(-1), Size(size), Num(0)
  {
    b = new T[Size];
  }

  ~ring()
  {
    delete[] b;
  }

  void push(const T &t) // push an element unto the buffer
  {
    if (Size) {
      if (++p == Size)
        p = 0;
      if (Num < Size)
        Num++;
      b[p] = t;
    }
  }

  T pop() // returns latest pushed element, and removes it
  {
    T temp;
    if (Num) {
      temp = b[p];
      if (--p < 0)
        p = Size - 1;
      Num--;
    }
    return temp;
  }

  const T& front() const // returns latest queued element, no pop
  {
    static T temp;
    if (Num)
      return b[p];
    else
      return temp;
  }

  T& operator[](const int index)
  {
    static T temp;
    if (index >= 0 && index < Num) {
      int i = p - index;
      if (i < 0)
        i += Size;
      return b[i];
    }
    return temp;
  }

  const T& operator[](const int index) const
  {
    static T temp;
    if (index >= 0 && index < Num) {
      int i = p - index;
      if (i < 0)
        i += Size;
      return b[i];
    }
    return temp;
  }

  int size() const // returns number of elements in buffer
  {
    return Num;
  }

  void clear() // clear buffer (elements are not deleted)
  {
    Num = 0;
    p = -1;
  }
};

#endif
