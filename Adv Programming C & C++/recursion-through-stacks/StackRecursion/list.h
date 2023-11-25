// Filename: List.h
// 
// Header file for the class List that represents a linked list
// 
// Japheth Frolick, August 2019
// C. Seshadhri, Jan 2020

#ifndef LIST_H
#define LIST_H
#include "node.h"
class List
{
	public:
		Node *head; // Stores head of linked list
		List(); // Default constructor sets head and tail to null
		void deleteList(); // frees all memory of list
        Node* find(string);  // find the node that contains string. If none exists, return NULL
        void insert(string); // Inserts word into list
        void sort(); // Sort list by increasing lex order
        string print(); // print the list into a string
        string print(string delim); // print the list with delimiter
};

#endif
