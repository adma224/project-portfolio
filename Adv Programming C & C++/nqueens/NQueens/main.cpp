#include <stack>
#include <string>
#include <vector>
#include "queens.h"
#include "util.h"
using namespace std;

int main(int argc, char** argv){
	ifstream in_file(argv[1]);
	FILE* out = fopen(argv[2],"w");
	Queens queens;
	int column, row, size;
	while(true){
		string line;
		getline(in_file,line);
		if(in_file.eof()) break;
		if(line=="") continue;
		wordvec words = split(line, " \t");
		if(words.size()%2==0) goto no_solution;
		else{
			size = stoi(words[0]);
			queens.set_size(size);
			for(int i=1;i<words.size();i+=2){
				column = stoi(words[i]);
				row = stoi(words[i+1]);
				if(column>size||row>size||column<0||row<0) goto no_solution;
				if(!queens.add(column,row)) goto no_solution;
			}
		}	
		if(queens.run_queens()){
			queens.print(out);
			fprintf(out,"\n");
		} else{
			no_solution:
			fprintf(out,"No solution\n");
		}
		queens.clear();
		words.clear();
	}
	fclose(out);
	return 0;
}
