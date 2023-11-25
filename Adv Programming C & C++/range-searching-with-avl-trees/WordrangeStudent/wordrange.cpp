#include "bst.h"
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cstring>

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        throw invalid_argument("Usage: ./wordrange <INPUT FILE> <OUTPUT FILE>"); 
    }

    ifstream input; 
    ofstream output; 

    input.open(argv[1]); 
    output.open(argv[2]); 

    string command; 
    char *com, *op, *val, *val2; 

    BST myBST; 
      
    while(getline(input,command)) 
    {
				
        if (command.length() == 0)
            continue;
        com = strdup(command.c_str()); 
        op = strtok(com, " \t");
				
        val = strtok(NULL, " \t"); 
        
        if(strcmp(op,"pin") == 0) 
        {
            output << myBST.printInOrder() << endl; // write printed tree to file, and add new line
            
            output << myBST.printInOrder() << endl; // printing to console
            continue; 
        }
        if(strcmp(op,"ppre") == 0) // print tree pre order
        {
            output << myBST.printPreOrder() << endl; // write printed tree to file, and add new line
            
            output << myBST.printPreOrder() << endl; // printing to console
            continue; // move on to next command
        }
        if(strcmp(op,"ppost") == 0) // print tree post order
        {
            output << myBST.printPostOrder() << endl; // write printed tree to file, and add new line
            output << myBST.printPostOrder() << endl; // printing to console
            continue; // move on to next command
        }
				string str1(val);
        if(strcmp(op,"i") == 0) 
        {
            myBST.insert(str1);
        }
        if(strcmp(op,"r") == 0) 
        {
          val2 = strtok(NULL," ");
          if(val2&&val){
            string str2(val2);
						output << myBST.range(str1,str2) << endl;
          }
        }
     }
     myBST.deleteBST(); 
     input.close();
     output.close();
}