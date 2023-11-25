## README for BinHeap codes ##

C. Seshadhri, Feb 2020

*****

### Instructions ###

The folder BinHeap contains a standard implementation of a binary maxheap, with a wrapper to run it.
It has an insert functionality, but not extractMax (that your job :D).
You can click on the file binheap_qns.pdf for a description of the test functions.

In the BinHeap directory, you will see the following files.
Makefile, binheap.cpp, binheap.h, heapwrapper.cpp, simple-input.txt, simple-output.txt 

USAGE (in BinHeap folder):
1) Run "make", to get executable "heapwrapper".
2) Run "./heapwrapper <INPUT FILE> <OUTPUT FILE>"
   
   Run "make clean" to delete executable and object files. A good practice to do this after changing code.


Later in the README, you can see how the input (in <INPUT FILE>) is structured.

The files simple-input.txt, simple-output.txt are example input and output files. In a correct code, the output should exactly match this output.
These files will help you test and debug your code.

----------------------------------------------------

### Input/output ###

The following explains the input/output format.

I/O:

The input file should have a series of operations in each line. Each line is of the form:

i <INT>: insert <INT> into heap
max: print maximum in heap
em: extract (delete) maximum from heap
p: print heap, as an array in order

All the printing is done in the <OUTPUT FILE> provided as a command line argument. Only the print
command leads to any printing in the output file.

Additionally, every command processed is printed to the console. Every printing is also done in the console.

----------------------------------------------------
