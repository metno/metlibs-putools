#include "miSetupParser.h"
#include <fstream>
#include <list>
#include <stdlib.h>

// static members
map<miString, miString> miutil::SetupParser::substitutions;
vector<miString> miutil::SetupParser::sfilename;
map<miString, miutil::SetupSection> miutil::SetupParser::sectionm;
map<miString, miString> miutil::SetupParser::user_variables;

using namespace miutil;
using namespace std;

void SetupParser::setUserVariables(const map<miString, miString> & user_var)
{
  user_variables = user_var;
}

bool SetupParser::checkSubstitutions(miString& t)
{
  if (!t.contains("$("))
    return false;

  int start, stop;

  start = t.find("$(", 0) + 2;
  stop = t.find(")", start);

  if (stop < start) {
    return false;
  }

  miString s = t.substr(start, stop - start);
  miString r = miString("$(") + s + ")";
  miString n;
  s = s.upcase();

  if (substitutions.count(s) > 0)
    n = substitutions[s];

  t.replace(r, n);

  //next substitution
  checkSubstitutions(t);

  return true;
}

bool SetupParser::checkEnvironment(miString& t)
{
  if (!t.contains("${"))
    return false;

  int start, stop;

  start = t.find("${", 0) + 2;
  stop = t.find("}", start);

  if (stop < start) {
    return false;
  }

  miString s = t.substr(start, stop - start);
  miString r = miString("${") + s + "}";

  s = s.upcase();

  miString n = getenv(s.cStr());

  t.replace(r, n);
  return true;
}

void SetupParser::cleanstr(miString& s)
{
  int p;
  if ((p = s.find("#")) != string::npos)
    s.erase(p);

  // substitute environment/shell variables
  checkEnvironment(s);

  // substitute local/setupfile variables
  checkSubstitutions(s);

  s.remove('\n');
  s.trim();

  // prepare strings for easy split on '=' and ' '
  // : remove leading and trailing " " for each '='
  if (s.contains("=") && s.contains(" ")) {
    p = 0;
    while ((p = s.find_first_of("=", p)) != string::npos) {
      // check for "" - do not clean out blanks inside these
      vector<int> sf1, sf2;
      int f1 = 0, f2;
      while ((f1 = s.find_first_of("\"", f1)) != string::npos && (f2
          = s.find_first_of("\"", f1 + 1)) != string::npos) {
        sf1.push_back(f1);
        sf2.push_back(f2);
        f1 = f2 + 1;
      }
      bool dropit = false;
      for (int i = 0; i < sf1.size(); i++) {
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

void SetupParser::splitKeyValue(const miString& s, miString& key,
    miString& value, bool keepCase)
{
  vector<miString> vs = s.split(2, '=', true);
  if (vs.size() == 2) {
    if (keepCase)
      key = vs[0];
    else
      key = vs[0].downcase();
    value = vs[1];
    // structures of type: A=B || C means A=B for existing B, otherwise C
    if (value.contains("||")) {
      int j = value.find("||");
      miString a1 = value.substr(0, j);
      miString a2 = value.substr(j + 2, value.length() - j);
      a1.trim();
      a2.trim();
      value = (a1.length() > 0 ? a1 : a2);
    }
    // remove "" from value
    if (value[0] == '"' && value[value.length() - 1] == '"')
      value = value.substr(1, value.length() - 2);
  } else if (vs.size() > 2) {
    if (keepCase)
      key = vs[0];
    else
      key = vs[0].downcase();
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
      key = s.downcase();
    value = "";
  }
}

void SetupParser::splitKeyValue(const miString& s, miString& key, vector<
    miString>& value)
{
  value.clear();
  vector<miString> vs = s.split(2, '=', true);
  if (vs.size() == 2) {
    key = vs[0].downcase(); // always converting keyword to lowercase !
    vector<miString> vv = vs[1].split(',', true);
    int n = vv.size();
    for (int i = 0; i < n; i++) {
      if (vv[i][0] == '"' && vv[i][vv[i].length() - 1] == '"')
        value.push_back(vv[i].substr(1, vv[i].length() - 2));
      else
        value.push_back(vv[i]);
    }
  } else {
    key = s.downcase(); // assuming pure keyword (without value)
  }
}

/*
 * parse one setupfile
 *
 */

bool SetupParser::parseFile(const miString& filename, // name of file
    const miString& section, // inherited section
    int level) // recursive level
{
  // list of filenames, index to them
  sfilename.push_back(filename);
  int activefile = sfilename.size() - 1;

  // ====== just output
  level++;
  miString dummy = " ";
  for (int i = 0; i <= level; i++)
    dummy += ".";
  cerr << dummy << " reading \t[" << filename << "] " << endl;
  // ===================

  const miString undefsect = "_UNDEF_";
  miString origsect = (section.exists() ? section : undefsect);
  miString sectname = origsect;
  list<miString> sectstack;

  miString str;
  int n, ln = 0, linenum;

  // open filestream
  ifstream file(filename.cStr());
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
  miString tmpstr;
  int tmpln;
  bool merge = false, newmerge;

  while (getline(file, str)) {
    ln++;
    str.trim();
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
        miString error = "Missing filename for include";
        internalErrorMsg(filename, linenum, error);
        return false;
      }
      miString nextfile = str.substr(8, n);
      nextfile.trim();
      if (!parseFile(nextfile, sectname, level))
        return false;

    } else if (str.upcase() == "CLEAR") {
      /*
       Clear all strings for this section
       Only valid inside a section
       */
      if (sectname == undefsect) {
        miString error = "CLEAR only valid within a section";
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
        miString key, value;
        splitKeyValue(str, key, value);
        if (value.exists()) {
          // Redefinitions are ignored
          if (substitutions.count(key.upcase()) == 0 ) {
            substitutions[key.upcase()] = value;
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
    miString error = "File started in section " + origsect
        + " and ended in section " + sectname;
    internalErrorMsg(filename, linenum, error);
    return false;
  }
  return true;
}

/*
 * Clears everything and parses a new setup file
 */
bool SetupParser::parse(const miString& mainfilename)
{
  sfilename.clear();
  sectionm.clear();
  substitutions.clear();

  // add user variables
  if (user_variables.size() > 0) {
     cerr << "SetupParser::parse, adding user variables:" << endl;
     map<miString, miString>::iterator itr = user_variables.begin();
     for (; itr != user_variables.end(); itr++) {
       cerr << itr->first << " = " << itr->second << endl;
       substitutions[itr->first.upcase()] = itr->second;
     }
   }

  if (!parseFile(mainfilename, "", -1))
    return false;

  return true;
}

// report an error with filename and linenumber
void SetupParser::internalErrorMsg(const miString& filename, const int linenum,
    const miString& error)
{
  cerr << "================================================" << endl;
  cerr << "Error in setupfile " << filename << endl
      << "The error occured in line " << linenum << endl << "Message: "
      << error << endl;
  cerr << "================================================" << endl;
}

// report an error with line# and sectionname
void SetupParser::errorMsg(const miString& sectname, const int linenum,
    const miString& error)
{
  map<miString, SetupSection>::iterator p;
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
void SetupParser::warningMsg(const miString& sectname, const int linenum,
    const miString& warning)
{
  map<miString, SetupSection>::iterator p;
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

bool SetupParser::getSection(const miString& sectname,
    vector<miString>& setuplines)
{
  map<miString, SetupSection>::iterator p;
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

