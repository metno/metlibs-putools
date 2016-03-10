/* -*- c++ -*-
  libpuTools - Basic types/algorithms/containers

  Copyright (C) 2013-2016 met.no

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

#ifndef PUTOOLS_MISTRINGBUILDER_H
#define PUTOOLS_MISTRINGBUILDER_H

#include <string>
#include <sstream>

namespace miutil {

class StringBuilder {
public:
    StringBuilder() { }

    template<typename T>
    StringBuilder& operator<<(const T& t)
        { s << t; return *this; }

    std::string str() const
        { return s.str(); }

    operator std::string() const
        { return s.str(); }

private:
    std::ostringstream s;
};

} // namespace miutil

#endif // PUTOOLS_MISTRINGBUILDER_H
