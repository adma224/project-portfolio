#include "trie.h"

TrieNode *trie_node_create(uint16_t code){
  TrieNode* n = calloc(1,sizeof(TrieNode));
  n->code = code;
  return n;
}

void trie_node_delete(TrieNode *n){
  free(n);
}


TrieNode *trie_create(){
  TrieNode* trie = trie_node_create(0);
  int i;
  for(i=0;i<256;i++){
    trie->children[i] = trie_node_create(i);
  }
  return trie;
}

void trie_recursive(TrieNode* n){
  TrieNode* child;
  int i;
  for(i=0;i<256;i++){
    child=n->children[i];
    if(child) trie_recursive(child);
  }
  trie_node_delete(n);
}

void trie_reset(TrieNode *root){
  int i;
  for(i=0;i<256;i++) trie_recursive(root->children[i]);
  for(i=0;i<256;i++) root->children[i]=trie_node_create(i);
}


void trie_delete(TrieNode *n){
  int i;
  for(i=0;i<256;i++) trie_recursive(n->children[i]);
  trie_node_delete(n);
}

TrieNode *trie_step(TrieNode *n, uint8_t sym){
  if(n->children[sym]) return n->children[sym];
  else return NULL;
}

