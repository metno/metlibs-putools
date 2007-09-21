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


#ifndef _ttycols_h
#define _ttycols_h

#include "miString.h"


/* Created by met.no/FoU/PU 
   at Fri Nov  1 10:07:39 2002 */

namespace ttc {
  
  enum attri_ { Plain, Bold, Underscore, Blink, Reverse, Concealed     };
  enum color_ { Black, Red , Green, Yellow, Blue, Magenta, Cyan, White };

  // reset all tty Colors/Attributes to normal
  const miString reset = "[0;0;0m";


  // Foreground , Attribute
  miString color(ttc::color_, ttc::attri_ = ttc::Plain );

  // Foreground, Background, Attribute
  miString color(ttc::color_, ttc::color_, ttc::attri_ = ttc::Plain );


  // Internal stuff ...
  miString foreground( ttc::color_ );
  miString background( ttc::color_ );
  miString attribute(  ttc::attri_ );

};

#endif
