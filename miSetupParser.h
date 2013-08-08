/*
  Copyright (C) 2013 met.no

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

#ifndef MISETUPPARSER_H
#define MISETUPPARSER_H

#include "miString.h"

#include <map>
#include <string>
#include <vector>

namespace miutil{

  struct KeyValue {
    std::string key, value;
    KeyValue() { }
    KeyValue(const std::string& k, const std::string& v) : key(k), value(v) { }
  };

  struct SetupSection {
    std::vector<std::string> strlist;
    std::vector<int> linenum;
    std::vector<int> filenum;
  };

  class SetupParser {
  private:
    /// list of setup-filenames
    static std::vector<std::string> sfilename;
    /// Setuptext hashed by Section name
    static std::map<std::string,SetupSection> sectionm;

    static std::map<std::string,std::string>    substitutions;

    static std::map<std::string, std::string> user_variables;

    /// report an error with filename and linenumber
    static void internalErrorMsg(const std::string& filename,
			  const int linenum,
			  const std::string& error);
    // expand local variables in string
    static bool checkSubstitutions(std::string& t);
    // expand environment values in string
    static bool checkEnvironment(std::string& t);
    /// parse one setupfile
    std::vector<std::string> getFromHttp(std::string url);
    std::vector<std::string> getFromFile(std::string filename);

    static bool parseFile(const std::string& filename,
		   const std::string& section,
		   int level);

  public:

    SetupParser() {}

    /// set user variables
    static void setUserVariables(const std::map<std::string,std::string> & user_var);
    /// replace or add user variables
    static void replaceUserVariables(const std::string& key, const std::string& value);
    /// get list of user variables
    static std::map<std::string,std::string> getUserVariables() {return substitutions;}
    /// cleans a string
    static void cleanstr(std::string&);
    /// finds key=value in string
    static void splitKeyValue(const std::string& s, std::string& key,
			      std::string& value, bool keepCase = false);
    static KeyValue splitKeyValue(const std::string& s, bool keepCase = false)
      { KeyValue kv; splitKeyValue(s, kv.key, kv.value, keepCase); return kv; }
    static std::vector<KeyValue> splitManyKeyValue(const std::string& line, bool keepCase = false);

    /// finds key=v1,v2,v3,... in string
    static void splitKeyValue(const std::string& s, std::string& key,
			      std::vector<std::string>& value);

    /// recursively parse setupfiles
    static bool parse(const std::string& mainfilename);
    /// get stringlist for a named section
    static bool getSection(const std::string&,std::vector<std::string>&);
    /// clear the section map sectionm (now used in tsData, ptGribStream)
    void clearSect();
    /// report an error with line# and sectionname
    static void errorMsg(const std::string&,const int,const std::string&);
    /// report a warning with line# and sectionname
    void warningMsg(const std::string&,const int,const std::string&);


    // miString compatibility functions
    static inline bool getSection(const std::string& s, std::vector<miutil::miString>& r)
          { std::vector<std::string> rs; bool ok = getSection(s, rs); r = std::vector<miutil::miString>(rs.begin(), rs.end()); return ok; }
    static inline void splitKeyValue(const std::string& s, std::string& k, std::vector<miutil::miString>& v)
          { std::vector<std::string> vs; splitKeyValue(s, k, vs); v = std::vector<miutil::miString>(vs.begin(), vs.end()); }
  };

}; // namespace miutil

#endif
