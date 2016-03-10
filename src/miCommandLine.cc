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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "miCommandLine.h"

#include <cstring>

using namespace std;

miCommandLine::miCommandLine(const vector<option>& o, const int argc, char** argv)
  : opts(o)
{
  for (int i=1; i<argc; i++) {
    if (*(argv[i])=='-') {
      if (strlen(argv[i])==1) {
        illegalOptionError(argv[0], argv[i]);
        continue;
      }

      if (*(argv[i]+1)!='-' && strlen(argv[i])>2) {
        illegalOptionError(argv[0], (argv[i]+1));
      } else {
        char newflag;
        if (*(argv[i]+1)=='-') { // alias
          newflag=aliasToFlag(argv[i]+2);
          if (!newflag) {
            illegalOptionError(argv[0], argv[i]);
            continue;
          }
        } else {
          newflag=*(argv[i]+1);

          if (!flagLegal(newflag)) {
            illegalOptionError(argv[0], newflag);
            continue;
          }
        }

        if (args.count(newflag)==0)
          args[newflag]=vector<string>();

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
}

char miCommandLine::aliasToFlag(const string& s) const
{
  for (unsigned int i=0; i<opts.size(); i++)
    if (opts[i].alias==s)
      return opts[i].flag;
  return '\0';
}

bool miCommandLine::flagLegal(char c) const
{
  for (unsigned int i=0; i<opts.size(); i++)
    if (opts[i].flag==c)
      return true;
  return false;
}

bool miCommandLine::hasArg(char c) const
{
  for (unsigned int i=0; i<opts.size(); i++)
    if (opts[i].flag==c)
      return opts[i].hasArg;
  return false;
}

void miCommandLine::illegalOptionError(const char* prog, const char opt)
{
  std::ostringstream est;
  est << prog << ": unknown option `" << opt << "'" << endl;
  errors += est.str();
}

void miCommandLine::illegalOptionError(const char* prog, const char* opt)
{
  std::ostringstream est;
  est << prog << ": unknown option `" << opt << "'"<< endl;
  errors += est.str();
}


void miCommandLine::requiresArgumentError(const char* prog, const char opt)
{
  std::ostringstream est;
  est << prog << ": option `" << opt << "' requires an argument"<< endl;
  errors += est.str();
}


void miCommandLine::requiresArgumentError(const char* prog, const char* opt)
{
  std::ostringstream est;
  est << prog << ": option `" << opt << "' requires an argument"<< endl;
  errors += est.str();
}

static const std::vector<std::string> EMPTY;

const std::vector<std::string>& miCommandLine::arg(char flag) const
{
  std::map<char,std::vector<std::string> >::const_iterator it = args.find(flag);
  if (it != args.end())
    return it->second;
  else
    return EMPTY;
}
