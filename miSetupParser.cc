#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "miSetupParser.h"
#include "miString.h"

#include <curl/curl.h>

#include <iostream>
#include <fstream>
#include <list>
#include <cstdlib>

using namespace std;
using namespace miutil;

// static members
map<std::string, std::string> miutil::SetupParser::substitutions;
vector<std::string> miutil::SetupParser::sfilename;
map<std::string, miutil::SetupSection> miutil::SetupParser::sectionm;
map<std::string, std::string> miutil::SetupParser::user_variables;

void SetupParser::setUserVariables(const map<std::string, std::string> & user_var)
{
  user_variables = user_var;
}

void SetupParser::replaceUserVariables(const std::string& key, const std::string& value)
{
  user_variables[key] = value;
}

bool SetupParser::checkSubstitutions(std::string& t)
{
  std::string::size_type start = 0, stop = 0;

  while ((start = t.find("$(", 0)) != t.npos) {
    if ((stop = t.find(")", start)) == t.npos) {
      // unterminated
      return false;
    }
    std::string s = t.substr(start + 2, stop - start - 2);
    std::string n;
    s = miutil::to_upper(s);
    if (substitutions.count(s) > 0) {
      n = substitutions[s];
    }
    // this would be the logical solution, but std::string overrides replace()
    // miutil::replace(t, start, stop - start + 1, n.c_str());
    t = t.substr(0, start) + n + t.substr(stop + 1);

  }
  return true;
}

bool SetupParser::checkEnvironment(std::string& t)
{
  std::string::size_type start = 0, stop = 0;

  while ((start = t.find("${", stop)) != t.npos) {
    if ((stop = t.find("}", start)) == t.npos)
      // unterminated
      return false;
    std::string s = t.substr(start + 2, stop - start - 2);
    std::string n;
    s = miutil::to_upper(s);
    if (substitutions.count(s) > 0) {
      n = substitutions[s];
    } else {
      n = miutil::from_c_str(getenv(s.c_str()));
    }
    // this would be the logical solution, but std::string overrides replace()
    // miutil::replace(t, start, stop - start + 1, n.c_str());
    t = t.substr(0, start) + n + t.substr(stop + 1);
  }
  return true;
}

void SetupParser::cleanstr(std::string& s)
{
  std::string::size_type p;
  if ((p = s.find("#")) != string::npos)
    s.erase(p);

  // substitute environment/shell variables
  checkEnvironment(s);

  // substitute local/setupfile variables
  checkSubstitutions(s);

  miutil::remove(s, '\n');
  miutil::trim(s);

  // prepare strings for easy split on '=' and ' '
  // : remove leading and trailing " " for each '='
  if (miutil::contains(s, "=") && miutil::contains(s, " ")) {
    p = 0;
    while ((p = s.find_first_of("=", p)) != string::npos) {
      // check for "" - do not clean out blanks inside these
      vector<int> sf1, sf2;
      std::string::size_type f1 = 0, f2;
      while ((f1 = s.find_first_of("\"", f1)) != string::npos && (f2
          = s.find_first_of("\"", f1 + 1)) != string::npos) {
        sf1.push_back(f1);
        sf2.push_back(f2);
        f1 = f2 + 1;
      }
      bool dropit = false;
      for (size_t i = 0; i < sf1.size(); i++) {
        f1 = sf1[i];
        f2 = sf2[i];
        if (f1 > p) {
          // no chance of "" pairs around this..
          break;
        }
        if (f1 < p && f2 > p) {
          p = f2 + 1;
          // '=' is inside a "" pair, drop cleaning
          dropit = true;
          break;
        }
      }
      if (dropit)
        continue;

      while (p > 0 && s[p - 1] == ' ') {
        s.erase(p - 1, 1);
        p--;
      }
      while (p < s.length() - 1 && s[p + 1] == ' ') {
        s.erase(p + 1, 1);
      }
      p++;
    }
  }

}

void SetupParser::splitKeyValue(const std::string& s, std::string& key,
    std::string& value, bool keepCase)
{
  vector<std::string> vs = miutil::split(s, 2, "=", true);
  if (vs.size() == 2) {
    if (keepCase)
      key = vs[0];
    else
      key = miutil::to_lower(vs[0]);
    value = vs[1];
    // structures of type: A=B || C means A=B for existing B, otherwise C
    if (miutil::contains(value, "||")) {
      int j = value.find("||");
      std::string a1 = value.substr(0, j);
      std::string a2 = value.substr(j + 2, value.length() - j);
      miutil::trim(a1);
      miutil::trim(a2);
      value = (a1.length() > 0 ? a1 : a2);
    }
    // remove "" from value
    if (value[0] == '"' && value[value.length() - 1] == '"')
      value = value.substr(1, value.length() - 2);
  } else if (vs.size() > 2) {
    if (keepCase)
      key = vs[0];
    else
      key = miutil::to_lower(vs[0]);
    int n = vs.size();
    value.clear();
    for (int i = 1; i < n; i++) {
      value += vs[i];
      if (i < n - 1)
        value += "=";
    }
    if (value[0] == '"' && value[value.length() - 1] == '"')
      value = value.substr(1, value.length() - 2);
  } else {
    if (keepCase)
      key = s;
    else
      key = miutil::to_lower(s);
    value = "";
  }
}

void SetupParser::splitKeyValue(const std::string& s, std::string& key, vector<
    std::string>& value)
{
  value.clear();
  vector<std::string> vs = miutil::split(s, 2, "=", true);
  if (vs.size() == 2) {
    key = miutil::to_lower(vs[0]); // always converting keyword to lowercase !
    vector<std::string> vv = miutil::split(vs[1], 0, ",", true);
    int n = vv.size();
    for (int i = 0; i < n; i++) {
      if (vv[i][0] == '"' && vv[i][vv[i].length() - 1] == '"')
        value.push_back(vv[i].substr(1, vv[i].length() - 2));
      else
        value.push_back(vv[i]);
    }
  } else {
    key = miutil::to_lower(s); // assuming pure keyword (without value)
  }
}

/*
 * parse one setupfile
 *
 */

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
  (*(ostringstream*) userp) << (char *) buffer;

  return (size_t) (size * nmemb);
}

vector<std::string> SetupParser::getFromHttp(std::string url)
{
  CURL *curl = NULL;
  CURLcode res;
  ostringstream ost;
  vector<std::string> result;

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ost);
    res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
  }

  std::string data = ost.str();

  //must contain diana.setup tags
  if (data.find("<diana.setup>") == data.npos || data.find("</diana.setup>") == data.npos) {
    cerr <<"WARNING: SetupParser::getFromHttp: "<<url
        <<": <diana.setup> or </diana.setup> tags are missing"<<endl;
    return result;
  }

  data = data.substr(data.find("<diana.setup>") + 13);
  data = data.substr(0,data.find("</diana.setup>"));

  result = miutil::split(data, "\n");

  return result;
}

vector<std::string> SetupParser::getFromFile(std::string filename)
{

  vector<std::string> result;

  // open filestream
  ifstream file(filename.c_str());
  if (!file) {
    cerr << "SetupParser::readSetup. cannot open setupfile " << filename
    << endl;
    return result;
  }


  std::string str;
  while (getline(file, str)) {
    result.push_back(str);
  }

  file.close();
  return result;

}

bool SetupParser::parseFile(const std::string& filename, // name of file
    const std::string& section, // inherited section
    int level) // recursive level
{
  // list of filenames, index to them
  sfilename.push_back(filename);
  int activefile = sfilename.size() - 1;

  // ====== just output
  level++;
  std::string dummy = " ";
  for (int i = 0; i <= level; i++)
    dummy += ".";
  cerr << dummy << " reading \t[" << filename << "] " << endl;
  // ===================

  const std::string undefsect = "_UNDEF_";
  std::string origsect = (not section.empty() ? section : undefsect);
  std::string sectname = origsect;
  list<std::string> sectstack;

  std::string str;
  int n, ln = 0, linenum;

  // open filestream
  ifstream file(filename.c_str());
  if (!file) {
    cerr << "SetupParser::readSetup. cannot open setupfile " << filename
        << endl;
    return false;
  }

  /*
   - skip blank lines,
   - strip lines for comments and left/right whitespace
   - merge lines ending with \
    - accumulate strings for each section
   */
  std::string tmpstr;
  int tmpln=0;
  bool merge = false, newmerge;

  while (getline(file, str)) {
    ln++;
    miutil::trim(str);
    n = str.length();
    if (n == 0)
      continue;

    /*
     check for linemerging
     */
    newmerge = false;
    if (str[n - 1] == '\\') {
      newmerge = true;
      str = str.substr(0, str.length() - 1);
    }
    if (merge) { // this is at least the second merge-line..
      tmpstr += str;
      if (newmerge)
        continue; // and there is more, go to next line
      str = tmpstr; // We are finished: go to checkout
      linenum = tmpln;
      merge = false;

    } else if (newmerge) { // This is the start of a merge
      tmpln = ln;
      tmpstr = str;
      merge = true;
      continue; // go to next line

    } else { // no merge at all
      linenum = ln;
    }

    /*
     Remove preceding and trailing blanks.
     Remove comments
     Remove blanks around '='
     Do variable substitutions
     */
    cleanstr(str);
    n = str.length();
    if (n == 0)
      continue;

    /*
     Check each line..
     */
    if (n > 1 && str[0] == '<' && str[n - 1] == '>') {
      // start or end of section
      if (str[1] == '/') { // end of current section
        if (sectstack.size() > 0) {
          // retreat to previous section
          sectname = sectstack.back();
          sectstack.pop_back();
        } else
          sectname = undefsect;

      } else { // start of new section
        // push current section onto stack
        sectstack.push_back(sectname);
        sectname = str.substr(1, n - 2);
      }

    } else if (str.substr(0, 8) == "%include") {
      /*
       include another setupfile
       */
      if (n < 10) {
        std::string error = "Missing filename for include";
        internalErrorMsg(filename, linenum, error);
        return false;
      }
      std::string nextfile = str.substr(8, n);
      miutil::trim(nextfile);
      if (!parseFile(nextfile, sectname, level))
        return false;

    } else if (miutil::to_upper(str) == "CLEAR") {
      /*
       Clear all strings for this section
       Only valid inside a section
       */
      if (sectname == undefsect) {
        std::string error = "CLEAR only valid within a section";
        internalErrorMsg(filename, linenum, error);
        continue;
      }
      if (sectionm.count(sectname) == 0)
        continue;
      sectionm[sectname].strlist.clear();
      sectionm[sectname].linenum.clear();
      sectionm[sectname].filenum.clear();
      //       // also add clear command to section
      //       sectionm[sectname].strlist.push_back(str);
      //       sectionm[sectname].linenum.push_back(linenum);
      //       sectionm[sectname].filenum.push_back(activefile);

    } else {
      /*
       Add string to section.
       If undefined section, check instead for variable declaration
       */
      if (sectname == undefsect) {
        std::string key, value;
        splitKeyValue(str, key, value);
        if (not value.empty()) {
          // Redefinitions are ignored
          if (substitutions.count(miutil::to_upper(key)) == 0 ) {
            substitutions[miutil::to_upper(key)] = value;
          }
        } else {
          cerr << "** setupfile WARNING, line " << linenum << " in file "
              << filename
              << " is no variabledefinition, and is outside all sections:"
              << str << endl;
        }
        continue;
      }
      // add strings to appropriate section
      sectionm[sectname].strlist.push_back(str);
      sectionm[sectname].linenum.push_back(linenum);
      sectionm[sectname].filenum.push_back(activefile);
    }
  }

  file.close();

  // File should start and end in same section
  if (sectname != origsect) {
    std::string error = "File started in section " + origsect
        + " and ended in section " + sectname;
    internalErrorMsg(filename, linenum, error);
    return false;
  }
  return true;
}

void SetupParser::clearSect()
{
  if (!sectionm.empty())
    sectionm.clear();

}

/*
 * Clears everything and parses a new setup file
 */
bool SetupParser::parse(const std::string& mainfilename)
{
  sfilename.clear();
  sectionm.clear();
  substitutions.clear();

  // add user variables
  if (user_variables.size() > 0) {
     cerr << "SetupParser::parse, adding user variables:" << endl;
     map<std::string, std::string>::iterator itr = user_variables.begin();
     for (; itr != user_variables.end(); itr++) {
       cerr << itr->first << " = " << itr->second << endl;
       substitutions[miutil::to_upper(itr->first)] = itr->second;
     }
   }

  if (!parseFile(mainfilename, "", -1))
    return false;

  return true;
}

// report an error with filename and linenumber
void SetupParser::internalErrorMsg(const std::string& filename, const int linenum,
    const std::string& error)
{
  cerr << "================================================" << endl;
  cerr << "Error in setupfile " << filename << endl
      << "The error occured in line " << linenum << endl << "Message: "
      << error << endl;
  cerr << "================================================" << endl;
}

// report an error with line# and sectionname
void SetupParser::errorMsg(const std::string& sectname, const int linenum,
    const std::string& error)
{
  map<std::string, SetupSection>::iterator p;
  if ((p = sectionm.find(sectname)) != sectionm.end()) {
    int n = p->second.linenum.size();
    int lnum = (linenum >= 0 && linenum < n) ? p->second.linenum[linenum]
        : 9999;
    int m = p->second.filenum.size();
    int fnum = (linenum >= 0 && linenum < m) ? p->second.filenum[linenum] : 0;

    cerr << "================================================" << endl;
    cerr << "Error in setupfile " << sfilename[fnum] << endl
        << "The error occured in section " << sectname << ", line " << lnum
        << endl << "Line   : " << p->second.strlist[linenum] << endl
        << "Message: " << error << endl;
    cerr << "================================================" << endl;
  } else {
    cerr << "================================================" << endl;
    cerr << "Internal SetupParser error." << endl
        << "An error was reported for unknown section " << sectname << endl
        << "The message is: " << error << endl;
    cerr << "================================================" << endl;
  }
}

// give a warning with line# and sectionname
void SetupParser::warningMsg(const std::string& sectname, const int linenum,
    const std::string& warning)
{
  map<std::string, SetupSection>::iterator p;
  if ((p = sectionm.find(sectname)) != sectionm.end()) {
    int n = p->second.linenum.size();
    int lnum = (linenum >= 0 && linenum < n) ? p->second.linenum[linenum]
        : 9999;
    int m = p->second.filenum.size();
    int fnum = (linenum >= 0 && linenum < m) ? p->second.filenum[linenum] : 0;

    cerr << "================================================" << endl;
    cerr << "Warning for setupfile " << sfilename[fnum] << endl << "Section "
        << sectname << ", line " << lnum << endl << "Line   : "
        << p->second.strlist[linenum] << endl << "Message: " << warning << endl;
    cerr << "================================================" << endl;
  } else {
    cerr << "================================================" << endl;
    cerr << "Internal SetupParser warning." << endl
        << "A warning was reported for unknown section " << sectname << endl
        << "The message is: " << warning << endl;
    cerr << "================================================" << endl;
  }
}

bool SetupParser::getSection(const std::string& sectname,
    vector<std::string>& setuplines)
{
  map<std::string, SetupSection>::iterator p;
  if ((p = sectionm.find(sectname)) != sectionm.end()) {
    setuplines = p->second.strlist;
    return true;
  }
#ifdef DEBUGPRINT1
  cerr << "Warning: ++SetupParser::getSection for unknown or missing (from setupfile) section: " <<
  sectname << endl;
#endif
  setuplines.clear();
  return false;
}

