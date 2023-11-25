#ifndef LIST_H
#define LIST_H

#include <string>

using namespace std;

struct Node
{
	string word;
	int count;
	Node* prev;
	Node* next;
};

class LinkedList
{
	private:
		Node* head;
	public:
		LinkedList();
		Node* newNode(string);
		void add(string);
		void remove(Node*);
		void insert(Node*,Node*);
		string find(int);
		void deleteList();
		string print();
		int length();
};

#endif
