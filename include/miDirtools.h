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


#ifndef _miDirtools_h
#define _miDirtools_h

#include "miString.h"

#include <vector>

#include <libgen.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>
using namespace std;

// get the newest modificated file from catalog 'cat'

extern miString  getRecent(const miString& cat);



// get all filenames from catalog 'cat'
extern bool getFilenames(const miString& cat,
			 vector<miString>& names);

// get all files with extension 'ext' from catalog 'cat'
extern bool getFilenamesByExt(const miString& cat,
			      const miString& ext,
			      vector<miString>& names);

// get a real path out of related path (uses current directory)
// examples: ( you are in /pug/local/include )
// hardpath( ../../tmp ) : result /pug/tmp
// hardpath()            : result /pug/local/include
// hardpath(puTools)     : result /pug/local/include/puTools
// hardpath(/usr/local)  : result /usr/local

extern miString hardpath(const miString&);



#ifdef WITH_REGEXP
// get all files matching regexp 'reg' from catalog 'cat'

#ifdef linux
#include <regex.h>
#endif

bool getFilenamesByRegexp(const miString& cat,
			  const miString& reg,
			  vector<miString>& names)
{
  vector<miString> allnames;
#ifdef linux
  regex_t preg;
  int res= regcomp(&preg, reg.cStr(), REG_EXTENDED | REG_NOSUB);
  if (res!=0 || !getFilenames(cat, allnames))
    return false;

  int nmatch= 0;
  int eflags= 0;
  regmatch_t pmatch;
  for (unsigned int i=0; i<allnames.size(); i++){
    if (allnames[i]!="." && allnames[i]!=".."){
      res= regexec(&preg, allnames[i].cStr(), nmatch, &pmatch, eflags);
      if (res==0) names.push_back(allnames[i]);
    }
  }
  regfree(&preg);
#else
  char *creg= regcmp(reg.cStr(),(char*)0);
  char *result;
  if (!creg || !getFilenames(cat, allnames))
    return false;

  for (int i=0; i<allnames.size(); i++){
    if (allnames[i]!="." && allnames[i]!=".."){
      result= regex(creg, allnames[i].cStr());
      if (result) names.push_back(allnames[i]);
    }
  }
  free(creg);
#endif
  return true;
};

#endif
#endif

