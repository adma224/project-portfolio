// Filename: Node.cpp
// 
// Contains the class Node that represents a node in linked list
// 
// Japheth Frolick, August 2019
// C. Seshadhri, Jan 2020

#include "node.h"
#include <cstdlib>
#include <string>
using namespace std;

// Default constructor sets word to empty string
Node :: Node()
{
	word = "";
	next = NULL;
}

// Constructor with string input sets word and freq to 1
Node :: Node(string s)
{
	word = s;
	next = NULL;
}


