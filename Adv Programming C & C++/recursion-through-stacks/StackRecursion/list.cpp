// Filename: List.cpp
// 
// Contains the class List that represents a linked list
// 
// Japheth Frolick, August 2019
// Based on Java code written by C.Seshadhri
// C. Seshadhri, Jan 2020

#include "list.h"
#include <cstdlib>
#include <iostream>
#include <vector> 
#include <algorithm>
using namespace std;
// Default constructor sets head to null
List :: List()
{
	head = NULL;
}

// Inserts word into list, with frequency 1. 
// Input: String to insert into the linked list
// Output: Void, just inserts new node at head of the list.
void List :: insert(string s)
{
    Node *latest = new Node(s); // create new node that has string s and freq 1
    latest->next = head; // add latest node to start of the list
    head = latest;// make this the new head
	return; 
}

// Finds node with string s, if it exists
// Input: String s to be found
// Output: Pointer to node containing s. Return NULL if s is not in list

Node* List :: find(string s)
{
    Node *curr = head; // curr is the current Node as it progresses through the linked list. Initialized to head to start at the head of the list
    // curr will look over list. At every iteration, we will check if curr->data is val. If so, we are done. Otherwise, we proceed through the list.
    while(curr != NULL) //looping over list
    {
        if (curr->word == s) // found val, so return curr
            return curr;
        curr = curr->next; // otherwise, proceed through list
    }
    // if loop terminates, val not found
    return NULL;
}

// Sort lists, by increasing lexicographic order
// Input: void
// Output: void. The list is finally sorted. The algorithm used is selection sort

void List :: sort()
{
    Node *curr1 = head, *curr2 = head; // two pointers to do the sorting
    Node *min;
    string temp_word; 
    while(curr1 != NULL) // traverse list
    {
        curr2 = curr1; // start curr2 from the curr1
        min = curr2;
        while(curr2 != NULL) // start traversing
        {
            if(min->word > curr2->word)
            {
                min = curr2;
            }
            curr2 = curr2->next; // move curr2
        }
        // now swap
        temp_word = curr1->word;
        curr1->word = min->word;
        min->word = temp_word;

        curr1 = curr1->next;
    }
}

// Deletes every node to prevent memory leaks.
// Input: None
// Output: Void, just deletes every node
void List :: deleteList()
{
	Node *curr = head; // curr is ths current node as it progresses through the linked list. Initialized to head to start at the head of the list
	// If curr is NULL list is empty. Just Return.
	if(curr == NULL){
		return;
	}
	// Store curr->next, then delete curr
	while(curr->next!=NULL){
		Node *temp = curr->next;
		delete(curr);
		curr = temp;
	}
	delete(curr);
}

// Prints list in order, into string
// Input: None
// Output: string that has list in order
string List :: print()
{
	Node *curr = head; // curr is ths current node as it progresses through the linked list. Initialized to head to start at the head of the list
    string output = ""; // output string
	while(curr!=NULL){ // loop over list
        output = output + " " + curr->word; //append current word onto output string
		curr = curr->next; // proceed to next node
	}
    return output;
}
// Prints list in order, into string with desired delimiter
// Input: string delimiter
// Output: string that has list in order, with delim used as delimiter
string List :: print(string delim)
{
    if (head == NULL) // empty list
        return "";
    string output = head->word; // put in first word
	Node *curr = head->next; // curr is the current node as it progresses through the linked list. Initialized to head to start at the head of the list
	while(curr!=NULL){ // loop over list
        output = output + delim + curr->word; //append current word onto output string
		curr = curr->next; // proceed to next node
	}
    return output;
}

