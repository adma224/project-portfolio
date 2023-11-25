// Filename: Node.h
// 
// Header file for the class Node that represents a node in each linked list
// 
// Japheth Frolick, August 2019

#ifndef NODE_H
#define NODE_H
#include <string>
using namespace std;
class Node
{
	public:
		string word; // stores the word in the node
		Node *next;
		Node(); //default constructor (never used)
		Node(string); // Constructor creates Node with word
};

#endif
