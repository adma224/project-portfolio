#include "ll.h"
#include <stdio.h>

bool move_to_front;
GoodSpeak* gs_create(char* key,char* value){
	GoodSpeak* gs = calloc(1,sizeof(GoodSpeak));
	if(key){
		gs->old_speak = calloc(1,strlen(key)+1);
		strcpy(gs->old_speak,key);	}
	if(value){
		gs->new_speak = calloc(1,strlen(value)+1);
		strcpy(gs->new_speak,value);
	}
	return gs;
}
void gs_delete(GoodSpeak* gs){
	free(gs->old_speak);
	free(gs->new_speak);
	free(gs);

}
ListNode *ll_node_create(GoodSpeak *gs){
	ListNode* n = calloc(1,sizeof(ListNode));
	n->gs = gs;
	n->next = NULL;
	return n;
}
void ll_node_delete(ListNode *n){
	gs_delete(n->gs);
	free(n);
}
// CODE SOURCED FROM PROF DARREL LONG
void ll_delete(ListNode *head){
	if(head){
		ListNode* r;
		ListNode* p = head;
		while(p){
			r = p;
			p = p->next;
			ll_node_delete(r);
		}
	}
}
ListNode *ll_insert(ListNode **head, GoodSpeak *gs){
	if(gs){
		ListNode* n = ll_node_create(gs);
		if(head){
			if(!*head){
				*head = n;
			} else{
				n->next = *head;
				*head = n;
			}
		} else{
			*head = n;
		}
		return n;
	} else{
		return NULL;
	}
}
ListNode *ll_lookup(ListNode **head, const char *key){
	if(head&&key){
		ListNode* r = *head;
		ListNode* p;
		if(strcmp(key,r->gs->old_speak)==0) return *head;
		while(r!=NULL&&strcmp(key,r->gs->old_speak)!=0){
			p = r;
			r = r->next;
		}
		if(!r) return NULL;
		if(move_to_front){
			p->next = r->next;
			r->next = *head;
			*head = r;
		}
		return r;
	} else{
		return NULL;
	}
}

