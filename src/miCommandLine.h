/*
  libpuTools - Basic types/algorithms/containers

  Copyright (C) 2006-2013 met.no

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


/*
  miCommandLine -- a simple class to handle command line arguments to
  programs

  Authors: LBS/JSc 06/1999
  Part of puTools
*/

#ifndef _miCommandLine_h
#define _miCommandLine_h

#include "puToolsVersion.h"

#include <map>
#include <vector>
#include <string>
#include <sstream>

class miCommandLine {
public:

  struct option {
    char flag;
    std::string alias;
    bool hasArg;

    option()
      : flag(0), hasArg(false) {}
    option(char f, const std::string& a, bool h)
      : flag(f), alias(a), hasArg(h) {}
  };

private:
  std::string errors;
  std::vector<option> opts;
  std::map<char,std::vector<std::string> > args;

  char aliasToFlag(const std::string&) const;
  bool flagLegal(char) const;
  bool hasArg(char) const;

  void requiresArgumentError(const char*, const char*);
  void requiresArgumentError(const char*, const char);

  void illegalOptionError(const char*, const char*);
  void illegalOptionError(const char*, const char);

public:
  miCommandLine(const std::vector<option>&, const int, char**);

  const std::vector<std::string>& arg(char flag) const;

  const std::vector<std::string> arg(const std::string& alias) const
    { return arg(aliasToFlag(alias)); }

  bool hasFlag(char flag) const
    { return args.count(flag) > 0; }

  bool hasFlag(const std::string& alias) const
    { return hasFlag(aliasToFlag(alias)); }

  bool empty() const
    { return args.empty(); }

  bool error() const
    { return !errors.empty(); }

  const std::string& errStr()
    { return errors; }
};

#endif
