
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
    std::vector<miString> strlist;
    std::vector<int> linenum;
    std::vector<int> filenum;
  };

  class SetupParser {
  private:
    /// list of setup-filenames
    static std::vector<miString> sfilename;
    /// Setuptext hashed by Section name
    static std::map<miString,SetupSection> sectionm;
    
    static std::map<miString,miString>    substitutions;

    /// report an error with filename and linenumber
    void internalErrorMsg(const miString& filename,
			  const int linenum,
			  const miString& error);
    // expand local variables in string
    bool checkSubstitutions(miString& t);
    // expand environment values in string
    bool checkEnvironment(miString& t);
    /// parse one setupfile
    bool parseFile(const miString& filename,
		   const miString& section,
		   int level);


  public:
    SetupParser() {}
    
    /// cleans a string
    void cleanstr(miString&);
    /// finds key=value in string
    static void splitKeyValue(const miString& s, miString& key,
			      miString& value);
    /// finds key=v1,v2,v3,... in string
    static void splitKeyValue(const miString& s, miString& key,
			      std::vector<miString>& value);
    
    /// recursively parse setupfiles
    bool parse(const miString& mainfilename );
    /// get stringlist for a named section
    bool getSection(const miString&,std::vector<miString>&);
    /// report an error with line# and sectionname
    void errorMsg(const miString&,const int,const miString&);
    /// report a warning with line# and sectionname
    void warningMsg(const miString&,const int,const miString&);
  };

}; // namespace miutil

#endif
