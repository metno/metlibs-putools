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

#include "miDirtools.h"

#include "miStringFunctions.h"
#include <puCtools/stat.h>

#include <cstring>

#include <dirent.h>
#include <libgen.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#if defined(WITH_REGEXP) && defined(linux)
#include <regex.h>
#endif

using namespace std;

namespace miutil {
long path_ctime(const std::string& path)
{
  pu_struct_stat buf;
  if (pu_stat(path.c_str(), &buf) != 0)
    return 0;
  return buf.st_ctime;
}
} // namespace miutil

std::string getRecent(const std::string& cat)
{
  DIR *dirp;
  dirent *dp;
  std::string last;
  std::string c = cat + "/";
  int l = 0;

  if ((dirp= opendir(cat.c_str()))) {
    while ((dp= readdir(dirp))) {
      if (strlen(dp->d_name)>0) {
        std::string name = c + dp->d_name;
        struct stat st;
        stat(name.c_str(), &st);
        if ( l < st.st_ctime ) {
          l = st.st_ctime;
          last = name;
        }
      }
    }
    closedir(dirp);
  }
  return last;
}


bool getFilenames(const std::string& cat, vector<std::string>& names)
{
  DIR *dirp;
  dirent *dp;
  if (!(dirp= opendir(cat.c_str())))
    return false;

  while ((dp= readdir(dirp))) {
    if (strlen(dp->d_name) > 0)
      names.push_back(dp->d_name);
  }
  closedir(dirp);
  return true;
}


bool getFilenamesByExt(const std::string& cat,
    const std::string& ext, vector<std::string>& names)
{
  vector<std::string> allnames;
  if (!getFilenames(cat, allnames))
    return false;

  for (unsigned int i=0; i<allnames.size(); i++){
    if (allnames[i]!="." && allnames[i]!=".."){
      const vector<std::string> splitv = miutil::split(allnames[i], ".", false);
      if (splitv.size()) {
        const std::string& cext = splitv[splitv.size()-1];
        if (cext == ext)
          names.push_back(allnames[i]);
      }
    }
  }
  return true;
}


std::string hardpath(const std::string& fname)
{
  std::string cwd = getcwd(NULL,128);
  cwd+="/"+fname;
  vector<std::string> vcwd = miutil::split(cwd, "/", true);
  vector<std::string>::iterator itr = vcwd.begin();

  if ( fname == "")
    return cwd;
  else if ( fname.c_str()[0] ==  '/')
    return fname;
  cwd = "";

  for(; itr != vcwd.end(); itr++)
    if( *itr == ".." ){
      itr--;
      vcwd.erase(itr);
      vcwd.erase(itr);
      itr--;
    }
   for(itr = vcwd.begin(); itr != vcwd.end(); itr++)
     cwd+="/"+(*itr);
   return cwd;
}


bool getFilenamesByRegexp(const std::string& cat,
    const std::string& reg, std::vector<std::string>& names)
{
#if defined(WITH_REGEXP)
  std::vector<std::string> allnames;
#ifdef linux
  regex_t preg;
  int res= regcomp(&preg, reg.c_str(), REG_EXTENDED | REG_NOSUB);
  if (res!=0 || !getFilenames(cat, allnames))
    return false;

  int nmatch= 0;
  int eflags= 0;
  regmatch_t pmatch;
  for (unsigned int i=0; i<allnames.size(); i++){
    if (allnames[i]!="." && allnames[i]!=".."){
      res= regexec(&preg, allnames[i].c_str(), nmatch, &pmatch, eflags);
      if (res==0) names.push_back(allnames[i]);
    }
  }
  regfree(&preg);
#else // !linux
  char *creg= regcmp(reg.c_str(),(char*)0);
  char *result;
  if (!creg || !getFilenames(cat, allnames))
    return false;

  for (int i=0; i<allnames.size(); i++){
    if (allnames[i]!="." && allnames[i]!=".."){
      result= regex(creg, allnames[i].c_str());
      if (result) names.push_back(allnames[i]);
    }
  }
  free(creg);
#endif // !linux
  return true;
#else // !WITH_REGEXP
  return false;
#endif
}
