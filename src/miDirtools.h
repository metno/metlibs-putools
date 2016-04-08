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

#ifndef _miDirtools_h
#define _miDirtools_h

#include <string>
#include <vector>

namespace miutil {

/*! Determine the modification time for a given path.
 * \param path the path to check
 * \return the modification time, or 0 if not found
 */
long path_ctime(const std::string& path);

} // namespace miutil

// get the newest modificated file from catalog 'cat'
extern std::string getRecent(const std::string& cat);

// get all filenames from catalog 'cat'
extern bool getFilenames(const std::string& cat, std::vector<std::string>& names);

// get all files with extension 'ext' from catalog 'cat'
extern bool getFilenamesByExt(const std::string& cat,
    const std::string& ext, std::vector<std::string>& names);

// get a real path out of related path (uses current directory)
// examples: ( you are in /pug/local/include )
// hardpath( ../../tmp ) : result /pug/tmp
// hardpath()            : result /pug/local/include
// hardpath(puTools)     : result /pug/local/include/puTools
// hardpath(/usr/local)  : result /usr/local

extern std::string hardpath(const std::string&);

// get all files matching regexp 'reg' from catalog 'cat'
// only functional if compiled with "WITH_REGEXP"
extern bool getFilenamesByRegexp(const std::string& cat,
    const std::string& reg, std::vector<std::string>& names);

#endif // _miDirtools_h
