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


#include "ttycols.h"

/* Created by met.no/PU 
   at Fri Nov  1 10:26:17 2002 */

using namespace std; 

static const miString ttcctrl = "[";


miString ttc::foreground(ttc::color_ c) 
{
  switch (c) {
  case Black:
    return "30";
  case Red : 
    return "31";
  case  Green: 
    return "32";
  case  Yellow:
    return "33"; 
  case  Blue: 
    return "34";
  case  Magenta: 
    return "35";
  case  Cyan: 
    return "36";
  case  White: 
    return "37";
  }
  return "30";
}

miString ttc::background(ttc::color_ c) 
{
  switch (c) {
  case Black:
    return "40";
  case Red : 
    return "41";
  case  Green: 
    return "42";
  case  Yellow:
    return "43"; 
  case  Blue: 
    return "44";
  case  Magenta: 
    return "45";
  case  Cyan: 
    return "46";
  case  White: 
    return "47";
  }
  return "40";
}


miString ttc::attribute( ttc::attri_ att ) 
{
  switch (att) {
  case  Plain:
    return "00;";
  case  Bold:
    return "01;";
  case  Underscore:
    return "04;";
  case  Blink:
    return "05;";
  case  Reverse:
    return "07;";
  case  Concealed:  
    return "08;";
  } 
  return "00;";
} 

miString ttc::color(ttc::color_ fg, ttc::attri_ att ) 
{
  return ttcctrl + attribute(att) + foreground(fg) + "m";
}

miString ttc::color(ttc::color_ fg, ttc::color_ bg, ttc::attri_ att) 
{
  return ttcctrl+attribute(att)+foreground(fg)+";"+background(bg)+"m";
}











