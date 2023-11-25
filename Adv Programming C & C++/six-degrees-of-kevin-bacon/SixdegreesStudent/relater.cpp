#include "graph.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

int main(int argc, char** argv){
  if (argc < 3)
  {
      throw invalid_argument("Usage: ./wordrange <INPUT FILE> <OUTPUT FILE>"); 
  }
  ifstream movies; 
  ifstream in;
  ofstream out;
  movies.open("cleaned_movielist.txt");  
  in.open(argv[1]);
  out.open(argv[2]);
  if(!in.good()||!out.good()||!movies.good()) exit(1);
  string line;
  Graph myGraph;
  int lines = 1;
  for(;;){
    try{
      getline(movies,line);
      if(movies.eof()) break;
      if(line.empty()) continue;
      wordvec words = split(line, " ");
      //for(size_t i=0; i<words.size(); i++){
      //  words[i] = replace(words[i],'_',' ');
      //}
      myGraph.insert(words);
    } catch(int e){
      continue;
    }
  }
  movies.close();
  cout << "Size of " << myGraph.size() << endl;
  while(!in.eof()&&!in.fail()){
    try{
      getline(in,line);
      if(line.empty()) continue;
      wordvec words = split(line, " ");
      if(words.size()==2){
        //words[0] = replace(words[0],'_',' ');
        //words[1] = replace(words[1],'_',' ');
        cout << endl<<lines++ << " " << line << endl;
        out << myGraph.degrees(words[0],words[1]) << endl;
      }
    } catch(int e){
      continue;
    }
  }
  myGraph.clear();
  in.close();
  out.close();
}