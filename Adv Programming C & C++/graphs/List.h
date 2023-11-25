//-----------------------------------------------------------------------------------------
// Author: 
// 	Adrian Murillo
// CruzID: 
// 	amurill9
// ID#: 
// 	1573260
// Institution:
// 	UC Santa Cruz
// Course:
// 	CMPS 101
// File Name:
// 	List.h
// File Description:
// 	Header file for List ADT with cursor
//-----------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

#ifndef _LIST_H_INCLUDE_
#define _LIST_H_INCLUDE_

struct rider{
	int data;
	struct rider *next;
	struct rider *prev;
} typedef node;

struct LIST{
	int index;
	node* cursor;
	node* head;
} typedef ListObj;

typedef ListObj* List;

// Constructors-Destructors ---------------------------------------------------
List newList(void);
void freeList(List* pL);

node *newNode(int d);
// Access functions -----------------------------------------------------------
int length(List L);
int index(List L);
int front(List L);
int back(List L);
int get(List L);
int equals(List A, List B);
// Manipulation procedures ----------------------------------------------------
void clear(List L);
void moveFront(List L);
void moveBack(List L);
void movePrev(List L);
void moveNext(List L);
void prepend(List L, int data);
void append(List L, int data);
void insertBefore(List L, int data);
void insertAfter(List L, int data);
void deleteFront(List L);
void deleteBack(List L);
void delete(List L);
// Other operations -----------------------------------------------------------
void printList(FILE* out, List L);
List copyList(List L);

#endif
