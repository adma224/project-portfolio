## README for BST codes ##

C. Seshadhri, Feb 2020

*****

### Instructions ###

The folder BST contains a standard implementation of a binary search tree (BST), with a wrapper to run it.
It has many basic functions implemented.
You can click on the file tree_qns.pdf for a description of the test functions.

In the BST directory, you will see the following files.
Makefile, bst.cpp, bst.h, treewrapper.cpp 

USAGE (in BST folder):
1) Run "make", to get executable "treewrapper".
2) Run "./treewrapper <INPUT FILE> <OUTPUT FILE>"
   
   Run "make clean" to delete executable and object files. A good practice to do this after changing code.


Later in the README, you can see how the input (in <INPUT FILE>) should be structured.

----------------------------------------------------

### Input/output ###

The following explains the input/output format.

I/O:

The input file should have a series of operations in each line. Each line is of the form:

i <INT>: insert <INT> into BST
f <INT>: find if <INT> is in BST
d <INT>: delete <INT> from BST
pin: print tree in order
ppre: print tree pre-order
ppost: print tree post-order

The find and print operations generate output, which is either "found/didn't find <INT>" or the tree printed in the desired order.
All the printing is done in the <OUTPUT FILE> provided as a command line argument. 

Additionally, every command processed is printed to the console. Every printing is also done in the console.

----------------------------------------------------
