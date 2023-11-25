#include "linkedList.h"
#include <string>
#include <iostream>
#include <cstring>

using namespace std;

LinkedList :: LinkedList(){
	head=NULL;
}

Node* LinkedList :: newNode(string str){
	Node* node = new Node;
	node->count = 1;
	node->word = str;
	node->next = NULL;
	node->prev = NULL;
	return node;
}

void LinkedList::remove(Node* node){
	if(node->prev) node->prev->next = node->next;
	if(node->next) node->next->prev = node->prev;
	node->prev = NULL;
	node->next = NULL;
}

void LinkedList::insert(Node* before, Node* node){
	if(before->next) before->next->prev = node;
	node->next = before->next;
	node->prev = before;
	before->next = node;
}

void LinkedList :: add(string str){
	if(head==NULL) head = newNode(str);
	else{
		Node* rider = head;
		Node* refer = NULL;
		while(rider){
			if(rider->word==str){
				rider->count++;
				goto reverse;
			}
			refer = rider;
			rider = rider->next;
		}
		rider = newNode(str);
		rider->prev = refer;
		if(refer) refer->next = rider;
	reverse:
		if(head==rider) return;
		refer = rider->prev;
		while(refer&&(rider->count>refer->count||(rider->word<refer->word&&rider->count==refer->count))){
			refer = refer->prev;
		}
		if(refer){
			if(refer->next==rider) return;
			remove(rider);
			insert(refer,rider);
		} else{
			remove(rider);
			rider->next = head;
			head->prev = rider;
			head = rider;
		}
	}
}

void LinkedList :: deleteList(){
	if(!head) return;
	Node* rider = head;
	Node* temp = NULL;
	while(rider!=NULL){
		temp = rider;
		rider = rider->next;
		delete(temp);
	}
	head = NULL;
}	

string LinkedList :: find(int position){
	string word = "";
	Node* rider = head;
	if(!rider) return "-";
	for(int j=0;(rider)&&(j<position);j++){
		rider=rider->next;
		if(!rider) return "-";
	}
	return rider->word;
}

string LinkedList :: print(){
	string list = "";
	Node* rider = head;
	while(rider){
		list = list + rider->word + ":"+ to_string(rider->count)+ " " ;
		rider = rider->next;
	}
	if(list.length() >0) list.pop_back(); 
	return list;
}

int LinkedList :: length(){
	int length = 0;
	Node* rider = head;
	while(rider){
		length++;
		rider = rider->next;
	}
	return length;
}
