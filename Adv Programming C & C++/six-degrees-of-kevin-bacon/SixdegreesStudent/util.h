#ifndef __UTIL_H__
#define __UTIL_H__

#include <vector>
#include <string>

using namespace std;
using wordvec = vector<string>;


wordvec split (const string&, const string&);

string& replace(string&, char, char);

#endif