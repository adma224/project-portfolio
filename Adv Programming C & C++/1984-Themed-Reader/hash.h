#ifndef _HASH_DL
#define _HASH_DL

#include <stdint.h>

uint64_t keyedHash(const char *, uint32_t, uint64_t *);

#endif

# ifndef NIL
# define NIL ( void *) 0
# endif

# ifndef __HASH_H__
# define __HASH_H__

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>
#include "ll.h"
#include "speck.h"

 //
 // Struct definition for a HashTable .
 //
 // salt : The salt of the HashTable ( used for hashing ).
 // length : The maximum number of entries in the HashTable .
 // heads : An array of linked list heads .
 //
typedef struct HashTable {
	uint64_t salt [2];
	uint32_t length ;
	ListNode ** heads ;
} HashTable ;

 //
 // Constructor for a HashTable .
 //
 // length : Length of the HashTable .
 // salt : Salt for the HashTable .
 //
HashTable * ht_create ( uint32_t length ) ;

 //
 // Destructor for a HashTable .
 //
 // ht: The HashTable .
 //
void ht_delete ( HashTable *ht) ;

 //
 // Searches a HashTable for a key .
 // Returns the ListNode if found and returns NULL otherwise .
 // This should call the ll_lookup () function .
 //
 // ht: The HashTable .
 // key: The key to search for .
 //
ListNode * ht_lookup ( HashTable *ht , const char * key ) ;

 //
 // First creates a new ListNode from GoodSpeak .
 // The created ListNode is then inserted into a HashTable .
 // This should call the ll_insert () function .
 //
 // ht: The HashTable .
 // gs: The GoodSpeak to add to the HashTable .
 //
void ht_insert ( HashTable *ht , GoodSpeak *gs) ;

# endif
