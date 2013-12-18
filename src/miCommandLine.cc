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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "miCommandLine.h"

#include <cstring>

using namespace std;
using namespace miutil;

miCommandLine::miCommandLine(const vector<option>& o,
			     const int argc, char** argv)
  : opts(o), err(false)
{
  int i;

  for (i=1; i<argc; i++)
    if (*(argv[i])=='-') {
      if (strlen(argv[i])==1) {
	illegalOptionError(argv[0], argv[i]);
	continue;
      }

      if (*(argv[i]+1)!='-' && strlen(argv[i])>2) {
	illegalOptionError(argv[0], (argv[i]+1));
// 	for (char* cp=argv[i]+1; *cp; *cp++) {
// 	  if (!flagLegal(*cp)) {
// 	    illegalOptionError(argv[0], *cp);
// 	    continue;
// 	  }
// 	  if (hasArg(*cp)) {
// 	    requiresArgumentError(argv[0], *cp);
// 	    continue;
// 	  }
// 	  args[*cp]=vector<miString>();
// 	}
      }
      else {
	char newflag;
	if (*(argv[i]+1)=='-') { // alias
	  newflag=aliasToFlag(argv[i]+2);
	  if (!newflag) {
	    illegalOptionError(argv[0], argv[i]);
	    continue;
	  }
	}
	else {
	  newflag=*(argv[i]+1);

	  if (!flagLegal(newflag)) {
	    illegalOptionError(argv[0], newflag);
	    continue;
	  }
	}

	if (args.count(newflag)==0)
	  args[newflag]=vector<miString>();

	if (hasArg(newflag)) {
	  for (i++; i<argc && *(argv[i])!='-'; i++)
	    args[newflag].push_back(argv[i]);
	  i--;
	  if (args[newflag].empty()) {
	    requiresArgumentError(argv[0],argv[i]);
	    args.erase(newflag);
	    continue;
	  }
	}
      }
    }
}

char miCommandLine::aliasToFlag(const miString& s) const
{
  for (unsigned int i=0; i<opts.size(); i++)
    if (opts[i].alias==s)
      return opts[i].flag;
  return '\0';
}

bool miCommandLine::flagLegal(const char c) const
{
  for (unsigned int i=0; i<opts.size(); i++)
    if (opts[i].flag==c)
      return true;
  return false;
}

bool miCommandLine::hasArg(const char c) const
{
  for (unsigned int i=0; i<opts.size(); i++)
    if (opts[i].flag==c)
      return opts[i].hasArg;
  return false;
}

void miCommandLine::illegalOptionError(const char* prog, const char opt)
{
  err=true;
  est << prog << ": unknown option `" << opt << "'" << endl;
}

void miCommandLine::illegalOptionError(const char* prog, const char* opt)
{
  err=true;
  est << prog << ": unknown option `" << opt << "'"<< endl;
}


void miCommandLine::requiresArgumentError(const char* prog, const char opt)
{
  err=true;
  est << prog << ": option `" << opt << "' requires an argument"<< endl;
}


void miCommandLine::requiresArgumentError(const char* prog, const char* opt)
{
  err=true;
  est << prog << ": option `" << opt << "' requires an argument"<< endl;
}


