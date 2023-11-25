#ifndef NIL
#define NIL (void *) 0
#endif
#ifndef MTF
#define MTF true
#endif

#ifndef __LL_H__
#define __LL_H__

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// If flag is set, ListNodes that are queried are moved to the front.
extern bool move_to_front;

typedef struct GoodSpeak {
	char * old_speak ;
	char * new_speak ;
} GoodSpeak ;

typedef struct ListNode ListNode;

//
// Struct definition of a ListNode.
//
// gs : GoodSpeak struct containing oldspeak and its newspeak translation.
//
struct ListNode {
  GoodSpeak *gs;
  ListNode *next;
};

GoodSpeak* gs_create(char* key,char* value);

//
// Constructor for a ListNode .
//
// gs : GoodSpeak struct containing oldspeak and its newspeak translation .
//
ListNode *ll_node_create(GoodSpeak *gs);

//
// Destructor for a ListNode.
//
// n : The ListNode to free.
//
void ll_node_delete(ListNode *n);

//
// Destructor for a linked list of ListNodes.
//
// head : The head of the linked list.
//
void ll_delete(ListNode *head);

//
// Creates and inserts a ListNode into a linked list.
//
// head : The head of the linked list to insert in.
// gs : GoodSpeak struct.
//
ListNode *ll_insert(ListNode **head, GoodSpeak *gs);

//
// Searches for a specific key in a linked list.
// Returns the ListNode if found and NULL otherwise.
//
// head : The head of the linked list to search in.
// key : The key to search for.
ListNode *ll_lookup(ListNode **head, const char *key);

#endif
