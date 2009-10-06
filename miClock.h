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


/* miClock.h

   Definition of miClock -- a class for manipulating clock times. In
   algebraic computations the number of seconds after midnight is used

   The default value of an miClock is `undef', and there is a function
   undef() to check for this state.

   Author: ØA/LBS met.no/FoU 1997, 1998, 1999
   Part of the puTools kit. */

#ifndef __dnmi_miClock_h__
#define __dnmi_miClock_h__ 

#include "miClock"

using namespace std;
using miutil::miClock;

#endif
