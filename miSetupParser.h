
#ifndef MISETUPPARSER_H
#define MISETUPPARSER_H

/*
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

#include "miString.h"
#include <vector>
#include <map>

namespace miutil{

  struct SetupSection {
    std::vector<miutil::miString> strlist;
    std::vector<int> linenum;
    std::vector<int> filenum;
  };

  class SetupParser {
  private:
    /// list of setup-filenames
    static std::vector<miutil::miString> sfilename;
    /// Setuptext hashed by Section name
    static std::map<miutil::miString,SetupSection> sectionm;

    static std::map<miutil::miString,miutil::miString>    substitutions;

    static std::map<miutil::miString, miutil::miString> user_variables;

    /// report an error with filename and linenumber
    static void internalErrorMsg(const miutil::miString& filename,
			  const int linenum,
			  const miutil::miString& error);
    // expand local variables in string
    static bool checkSubstitutions(miutil::miString& t);
    // expand environment values in string
    static bool checkEnvironment(miutil::miString& t);
    /// parse one setupfile
    std::vector<miutil::miString> getFromHttp(miutil::miString url);
    std::vector<miutil::miString> getFromFile(miutil::miString filename);

    static bool parseFile(const miutil::miString& filename,
		   const miutil::miString& section,
		   int level);

  public:

    SetupParser() {}

    /// set user variables
    static void setUserVariables(const std::map<miutil::miString,miutil::miString> & user_var);
    /// replace or add user variables
    static void replaceUserVariables(const miutil::miString& key, const miutil::miString& value);
    /// get list of user variables
    static std::map<miutil::miString,miutil::miString> getUserVariables() {return substitutions;}
    /// cleans a string
    static void cleanstr(miutil::miString&);
    /// finds key=value in string
    static void splitKeyValue(const miutil::miString& s, miutil::miString& key,
			      miutil::miString& value, bool keepCase = false);
    /// finds key=v1,v2,v3,... in string
    static void splitKeyValue(const miutil::miString& s, miutil::miString& key,
			      std::vector<miutil::miString>& value);

    /// recursively parse setupfiles
    static bool parse(const miutil::miString& mainfilename );
    /// get stringlist for a named section
    static bool getSection(const miutil::miString&,std::vector<miutil::miString>&);
    /// clear the section map sectionm (now used in tsData, ptGribStream)
    void clearSect();
    /// report an error with line# and sectionname
    static void errorMsg(const miutil::miString&,const int,const miutil::miString&);
    /// report a warning with line# and sectionname
    void warningMsg(const miutil::miString&,const int,const miutil::miString&);
  };

}; // namespace miutil

#endif
