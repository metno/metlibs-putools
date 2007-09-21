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


/* miDate.h

   Definition of miDate -- a class for manipulating Gregorian dates.
   Leap years, 400-year cycles, the `Year 2000 compliance' and so on,
   are dealt with by using the Julian day number (jdn) as the internal
   representation of the date, making this class able to perform any
   of its operations safely on dates ranging from about 5000000 BCE to
   5000000 CE or thereabouts.

   The standard C library (through time.h) is used in some functions.
   This means that in programs using these functions are `safe' (eg.
   for the future Y2038 dilemma) only to the extent that libc is; this
   safety is implementation-dependent.

   The default value of a date is `undef', and there is a function
   undef() to check for this state.

   Author: LBS. met.no/FoU 1997, 1998, 1999
   Part of the puTools kit. */

#ifndef __dnmi_miDate_h__
#define __dnmi_miDate_h__

#include "miDate"

using namespace std;
using miutil::miDate;

#endif
