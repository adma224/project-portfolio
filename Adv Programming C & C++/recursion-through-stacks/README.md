## README for StackRecursion codes ##

C. Seshadhri, Jan 2020

*****

### Basic description ###

The folder StackRecursion has functions that output all subsequences of an input string. A subsequence of a string is obtained
by taking any subset of its characters (in order). There are two implementations, one recursive, and one that uses a stack. 
The functions store all the subsequences in a linked list. The linked list code is the same implementation used in the previous
test.

This will be the setup used for your tests. You will need to write a function that processes a string and returns a list.
The file stackrecursion.pdf has a collection of test questions, with the declaration. You may want to get familiar with 
C++ string functions, especially those that access various characters in the string. (It's not the same as a C-style char array.)

----------------------------------------------------

### Files ###

This StackRecursion folder contains:

Code files: Makefile, stackrecursion.cpp, linkedlist.cpp, linkedlist.h, node.cpp, node.h
Extra files: README, stackrecursion_qns.pdf

______

### Usage ###


USAGE (in main folder):
1) Run "make", to get executable "stackrecursion".
2) Run "./stackrecursion <INPUT FILE> <OUTPUT FILE>"


----------------------------------------------------

### Input/output ###

The following explains the input/output format.

Only the first line of the input file is read, as a string. This is passed to the function that produces all subsequences in a list.
This list is sorted lexicographically and printed to the output file. In the output file, each subsequence is printed in a separate line.
Note that the empty string (with 0 characters) is also a subsequence.
