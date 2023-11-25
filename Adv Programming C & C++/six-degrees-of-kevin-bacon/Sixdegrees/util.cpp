#include "util.h"
#include <algorithm>

wordvec split(const string& line,const string& separator){
	wordvec words;
	size_t end=0;
	while(true){
		size_t start = line.find_first_not_of(separator,end);
		if(start==string::npos) break;
		end = line.find_first_of(separator, start);
		words.push_back(line.substr(start,end-start));
	}
	return words;
}

string& replace(string& line, char before, char after){
  replace(line.begin(), line.end(), before, after);
  return line;
}
