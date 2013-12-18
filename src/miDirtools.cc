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

#include <miDirtools.h>

using namespace std;
using namespace miutil;

miString  getRecent(const miString& cat)
{
  DIR *dirp;
  dirent *dp;
  miString last;
  miString c = cat + "/";
  int l = 0;

  if ((dirp= opendir(cat.c_str()))) {
      while ((dp= readdir(dirp))) {
      if (strlen(dp->d_name)>0) {
	miString name = c + dp->d_name;
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

bool getFilenames(const miString& cat,
		  vector<miString>& names)
{
  DIR *dirp;
  dirent *dp;
  if (!(dirp= opendir(cat.c_str())))
    return false;

  while ((dp= readdir(dirp))) {
    if (strlen(dp->d_name)>0)
      //       dp->d_namlen)
      names.push_back(dp->d_name);
  }
  closedir(dirp);
  return true;
}

bool getFilenamesByExt(const miString& cat,
		       const miString& ext,
		       vector<miString>& names)
{
  vector<miString> allnames, splitv;
  miString cext;
  if (!getFilenames(cat, allnames))
    return false;

  for (unsigned int i=0; i<allnames.size(); i++){
    if (allnames[i]!="." && allnames[i]!=".."){
      splitv= allnames[i].split('.');
      if (splitv.size()){
	cext= splitv[splitv.size()-1];
	if (cext == ext)
	  names.push_back(allnames[i]);
      }
    }
  }
  return true;
}




miString hardpath(const miString& fname){
  miString cwd =  getcwd(NULL,128);
  cwd+="/"+fname;
  vector<miString> vcwd = cwd.split('/');
  vector<miString>::iterator itr = vcwd.begin();

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
};
