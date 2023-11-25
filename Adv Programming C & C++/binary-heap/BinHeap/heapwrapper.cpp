// Filename: heapwrapper.cpp
//
// This is the main wrapper for the BinHeap class (based on LinkedList codes for CSE101).
// 
// After running make, the usage is:
//     ./heapwrapper <INPUT_FILE> <OUTPUT_FILE>
// 
// The input file contains a list of operations performed on a linked list. The operations are insert, max, delete max, print. 
// Each line is of the form "i <INT>", "max", "em", or "p". This means "insert", "(output) maximum", "extract maximum", or "print"
//  respectively. The second INT argument is for inserting that number.
// 
// Only the print operations generate output, which is the list printed in order in a single line.
// The output file contains the output of all print commands.
// 
// In addition, the console output shows the operations performed, and also prints the max or entire heap (for command that prints).
// Look at the associated README in this directory for more details.
// 
// C. Seshadhri, Jan 2020

#include <iostream>
#include "binheap.h"
#include <stack>
#include <stdexcept>
#include <fstream>
#include <array>
#include <vector> 
#include <algorithm>
#include <sstream>
#include <cstring>

using namespace std;


int main(int argc, char** argv)
{
    if (argc < 3) // must provide two arguments as input
    {
        throw std::invalid_argument("Usage: ./heapwrapper <INPUT FILE> <OUTPUT FILE>"); // throw error
    }

    ifstream input; // stream for input file
    ofstream output; // stream for output file

    input.open(argv[1]); // open input file
    output.open(argv[2]); // open output file

    string command; // to store the next command and operation
    char *com, *dummy, *valstr, *op; // for using with strtok, strtol
    int val; // the value from the command

    BinHeap myHeap; // initializing the linked list
        

    while(getline(input,command)) // get next line of input, store as repeat
    {
        if (command.length() == 0) // command is empty
            continue;
        com = strdup(command.c_str()); // annoying, first copy string into a "C-style" string
        op = strtok(com, " \t"); //tokenize command on whitespace, first token is operation

        // We first check if command is to print, since we do not need further parsing of command
        if(strcmp(op,"p") == 0) // print list
        {
            output << myHeap.print() << endl; // write printed list to file, and add new line
            cout << "Printing" << endl;
            cout << myHeap.print() << endl; // printing to console
            continue; // move on to next command
        }

        valstr = strtok(NULL, " \t"); // next token is value, as string (check out documentation on strtok, to understand the parsing)
        if(valstr != NULL) // first check if there is actually anything to convert into int
            val = strtol(valstr, &dummy, 10); // convert initial portion of valstr into an integer val, with base 10. Check out documentation on strtol for more details. 
    
        if(strcmp(op,"i") == 0) // insert into list
        {
            cout << "Insert "+to_string(val) << endl;
            myHeap.insert(val);
        }
        if(strcmp(op,"em") == 0)
        {
            myHeap.extractMax();
            cout << myHeap.print() << endl;
        }
        if(strcmp(op,"max") == 0) // delete from list
        {
            int max = myHeap.findMax();
            output << to_string(max) << endl; // write max to file
            cout << "Max: "+to_string(max) << endl;
        }

// --------------------------------------------------------------------------------------------------------------
        // CODE TO CALL EXTRACT MAX: Uncomment this, if you want to test your extract max
// --------------------------------------------------------------------------------------------------------------
//         if(strcmp(op,"em") == 0) // delete from list
//         {
//             cout << "Extract max" << endl;
//             myHeap.extractMax();
//         }
     }
     myHeap.deleteHeap(); // free the memory
     input.close();
     output.close();
}
