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
// 	List.c
// File Description:
// 	List ADT function implementations
//-----------------------------------------------------------------------------------------
#include <stdio.h>
#include "List.h"
//
//
//
//
//

// Constructors-Destructors ---------------------------------------------------
List newList(void){
	List L = malloc(sizeof(List)+1);
	L->index=-1;
	L->cursor=NULL;
	L->head=NULL;
	return L;
}
node *newNode(int d){
	node *node = malloc(sizeof(node));
	node->data = d;
	return node;
}
void freeList(List* pL){
	if(pL!=NULL&&*pL!=NULL) { 
		List L = *pL;
      	clear(L);
    	free(*pL);
		*pL = NULL;
   }
}

// Access functions -----------------------------------------------------------
int length(List L){
	node* rider = L->head;
	int length = 0;
	while(rider!=NULL){
		rider=rider->next;
		length++;
	}
	return length;
}
int index(List L){
	if(L->cursor!=NULL){
		return L->index;
	} else{
		return -1;
	}
}
int front(List L){
	if(length(L)>0){
		return L->head->data;
	} else{
		return -1;
	}
}
int back(List L){
	if(length(L)>0){
		node* rider = L->head;
		while(rider->next!=NULL){
			rider=rider->next;
		}
		return rider->data;
	} else{
		return -1;
	}
}
int get(List L){
	if(length(L)>0&&index(L)>=0){
		return L->cursor->data;
	} else{
		return -1;
	}
}
int equals(List A, List B){
	if(length(A)!=length(B)){
		return 0;
	}
	node* rider1 = A->head;
	node* rider2 = B->head;
	while(rider1!=NULL&&rider2!=NULL){
		if(rider1->data!=rider2->data){
			return 0;
		}
		rider1 = rider1->next;
		rider2 = rider2->next;
	}
	return 1;
}
// Manipulation procedures ----------------------------------------------------
void clear(List L){
	if(L!=NULL){
		node* rider = L->head;
	    while(rider!=NULL){
	     	free(rider);
	      	rider=rider->next;
	    }
	    L->head=NULL;
	    L->cursor=NULL;
	    L->index=-1;
	}
}
void moveFront(List L){
	if(length(L)>0){
		L->cursor=L->head;
		L->index=0;
	}
}
void moveBack(List L){
	L->cursor=L->head;
	int i =0;
	while(L->cursor->next!=NULL){
		i++;
		L->cursor=L->cursor->next;
	}
	L->index=i;
}
void movePrev(List L){
	if(L->cursor!=NULL&&L->cursor!=L->head){
		L->cursor=L->cursor->prev;
		L->index--;
	} else{
		L->cursor=NULL;
		L->index=-1;
	}
}
void moveNext(List L){
	if(L->cursor->next!=NULL&&L->cursor!=NULL){
		L->cursor=L->cursor->next;
		L->index++;
	} else{
		L->cursor=NULL;
		L->index=-1;
	}
}
void prepend(List L, int data){
	if(length(L)>0){
		node* n = newNode(data);
		n->next=L->head;
		L->head->prev=n;
		L->head=n;	
		L->index++;
	} else{
		node* n = newNode(data);
		L->head = n;
	} 
}
void append(List L, int data){
	if(length(L)>0){
		node* rider = L->head;
		while(rider->next!=NULL){
			rider=rider->next;
		}
		node* n = newNode(data);
		rider->next=n;
		n->prev=rider;
	} else{
		L->head=newNode(data);
	}
}
void insertBefore(List L, int data){
	if(length(L)>0&&index(L)>=0){
		node* n = newNode(data);
		if(L->cursor==L->head){
			n->next=L->cursor;
			L->cursor->prev=n;
			L->head=n;
		} else{
			node* temp = L->cursor->prev;
			temp->next=n;
			n->prev=temp;
			n->next=L->cursor;
			L->cursor->prev=n;
		}
		L->index++;
	}
}
void insertAfter(List L, int data){
	if(index(L)>=0&&length(L)>0){
		node* n = newNode(data);
		if(L->cursor->next!=NULL){
			node* temp = L->cursor->next;
			n->next=temp;
			temp->prev=n;
			L->cursor->next=n;
			n->prev=L->cursor;
		}else{
			L->cursor->next=n;
			n->prev=L->cursor;
		}
	}
}
void deleteFront(List L){
	if(length(L)>1&&L!=NULL) { 
		node* N = L->head;
   		L->head = L->head->next;
   		L->head->prev=NULL;
   		L->index--;
		free(N);
	} else if(length(L)==1&&L!=NULL){
		node* N = L->head;
		L->head=NULL;
		free(N);
	}
}
void deleteBack(List L){
	if(length(L)>0&&L!=NULL){
		if(length(L)==1){
			free(L->head);
			L->head=NULL;
			L->cursor=NULL;
			L->index=-1;
		} else{
			node* rider = L->head;
			while(rider->next!=NULL){
				rider=rider->next;
			}
			if(L->cursor==rider){
				L->cursor=NULL;
				L->index=-1;
			}
			node* prev = rider->prev;
			prev->next = NULL;
			free(rider);
			rider=NULL;
		}
	}
}
void delete(List L){
	if(length(L)>0&&index(L)>=0&&L!=NULL){
		if(length(L)==1){
			L->head=NULL;
		} else{
			node* delete = L->cursor;
			if(L->cursor==L->head){
				L->head=L->head->next;
				L->head->prev=NULL;
			}else if(L->cursor->next==NULL){
				L->cursor=L->cursor->prev;
				L->cursor->next=NULL;
			}else{
				node* prev=L->cursor->prev;
				node* next=L->cursor->next;
				prev->next=next;
				next->prev=prev;
			}
			free(delete);
			delete=NULL;
		}
		L->cursor=NULL;
		L->index=-1;
	}
}
// Other operations -----------------------------------------------------------
void printList(FILE* out, List L){
	node* rider = L->head;
	fprintf(out, "%i", rider->data);
	rider=rider->next;
	while(rider!=NULL){
		fprintf(out, " %i", rider->data);
		rider=rider->next;
	}
}
List copyList(List L){
	List L2 = newList();
	node* rider = L->head;
	while(rider!=NULL){
		append(L2,rider->data);
		rider=rider->next;
	}
	return L2;
}

