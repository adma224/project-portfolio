#include "linkedList.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

void exit_error(int err_num, string err_message){
	cerr << err_message << endl;
	exit(err_num);
}

int main(int argc, char** argv){
	if(argc!=3) exit_error(1,"Usage: ./bard <infile> <outfile>");
	ifstream william;
	william.open("shakespeare-cleaned5.txt",ios::in);
	if(!william) exit_error(2, "Refer Text File - Not Found");
	ifstream size;
	size.open("shakespeare-cleaned5.txt",ios::in);
	
	FILE* inFile = fopen(argv[1],"r");
	ofstream outFile;
	outFile.open(argv[2]);

	int max_length = 0;
	int length = 0;
	char letter;
	while(!size.eof()){
		size.get(letter);
		if(letter=='\n'){
			if(max_length<length){
				max_length = length;
			}
			length=0;
		}
		else length++;
	}

	
	LinkedList* array = new LinkedList[max_length+1];
	
	string buffer = "";
	length = 0;
	
	int words = 0;
	while(!william.eof()){
		william.get(letter);
		if(letter=='\n'){
			array[length].add(buffer);
			buffer = "";	
			words++;
			length=0;
		} else{
			buffer += letter;
			length++;
		}
	}
	
	int rank;
	string found;

	while(fscanf(inFile,"%i %i\n",&length,&rank)!=EOF){
		found = array[length].find(rank);
		outFile <<found;
		outFile <<endl;
	}
	

	
	for(int i=0;i<=max_length;i++) {
		array[i].deleteList();
	}

	delete(array);
	fclose(inFile);

	return 0;
}
