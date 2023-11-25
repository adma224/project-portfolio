// stack.h - Contains the function declarations for the Stack ADT .

#ifndef __STACK_H__
#define __STACK_H__

#include <inttypes.h>
#include <stdbool.h>

//
// Struct definition for a Stack .
//
// name : The Stack 's single - character identifier .
// top :  Keeps track of the top of the Stack .
// capacity : The maximum number of items a Stack can hold .
// items : The array to store Stack items in .
//
typedef struct Stack {
  char name;
  int top;
  int capacity;
  int *items;
} Stack;

//
// Constructor for a new Stack .
//
// capacity : The maximum number of items the Stack can hold .
// name : The Stack 's single - character identifier .
//
Stack *stack_create(int capacity, char name);

//
// Destructor for a Stack .
//
// s: The Stack to free allocated memory for .
//
void stack_delete(Stack *s);

//
// Pops an item off a Stack if it isn 't empty .
//
// s : The Stack to pop an item off of .
//
int stack_pop(Stack *s);

//
// Pushes an item into a Stack if it isn 't full .
//
// s : The Stack to push an item into .
//
void stack_push(Stack *s, int item);

//
// Returns true if a Stack is empty and false otherwise .
//
// s: The Stack to query about being empty .
bool stack_empty(Stack *s);

#endif
