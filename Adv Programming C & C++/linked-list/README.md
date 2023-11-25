## README for LinkedList codes ##

Japheth Frolick, Aug 2019
C. Seshadhri, Jan 2020

*****

### Basic description ###

These folders have linked list codes. The folder LinkedList contains a standard implementation of a linked list, with a wrapper to run it.
The subfolder within Functional/ has exactly the same function, except the implementation is purely functional (as much as C/C++ will allow). This is no looping, and all iteration is done through recursion.
Note the functionality is identical, and the codes should generate the exact same output.

----------------------------------------------------

### Files ###

This LinkedList folder contains a collection of codes for linked lists.

Code files: Makefile, linkedlist.cpp, linkedlist.h, listwrapper.cpp 
Input/output files: input.txt, output.txt 
Extra files: README, linkedlist.pdf

Also, in the folder Linkedlist/Functional, there are similar files.
Code files: Makefile, linkedlistfunc.cpp, linkedlistfunc.h, listwrapper.cpp 
Input/output files: input.txt, output.txt 

______

### Usage ###


USAGE (in main folder):
1) Run "make", to get executable "listwrapper".
2) Run "./listwrapper <INPUT FILE> <OUTPUT FILE>"

USAGE (in Functional/ folder): Identical to above
1) Run "make", to get executable "listwrapper".
2) Run "./listwrapper <INPUT FILE> <OUTPUT FILE>"

These codes are completely independent. The listwrapper code is identical for both. But (naturally) the headers and internal linked list codes are different.

----------------------------------------------------

### Input/output ###

The following explains the input/output format.

I/O:

The input file should have a series of operations in each line. Each line is of the form:
i <INT>: insert <INT> into list
d <INT>: delete <INT> from list
p: print list in order, as a single line

All the printing is done in the <OUTPUT FILE> provided as a command line argument. Additionally, every command processed is printed to the console. Every list printing is also done in the console.
Look at input.txt and output.txt to see examples.
