#ifndef __TRIE_H__
#define __TRIE_H__

#include <inttypes.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

typedef struct TrieNode TrieNode;

//
// Struct definition for a TrieNode.
//
// children:  An array of TrieNode pointers.
// code:      The TrieNode's code number.
//
struct TrieNode {
  TrieNode *children[256];
  uint16_t code;
};

//
// Constructor for a TrieNode.
//
// sym:    The TrieNode's symbol, or character.
// code:   The TrieNode's unique code.
//
TrieNode *trie_node_create(uint16_t code);

//
// Destructor for a TrieNode.
//
// n:  The TrieNode.
//
void trie_node_delete(TrieNode *n);

//
// Creates and initializes a new Trie.
// All ASCII characters are initially added as the root's children.
//
TrieNode *trie_create();

//
// Resets a trie back to its original state of only ASCII characters .
//
// root : The root of the trie to reset .
//
void trie_reset ( TrieNode * root ) ;

//
// Destructor for a TrieNode.
//
// n:  The TrieNode.
//
void trie_delete(TrieNode *n);

//
// Returns the TrieNode one step down the Trie for the next specified symbol.
// Returns NULL if the symbol doesn't exist.
//
// n:     The TrieNode to step down from.
// sym:   The symbol to step down to.
//
TrieNode *trie_step(TrieNode *n, uint8_t sym);

#endif
