#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

Stack* stack_create(int capacity, char name){
	Stack* stack = (Stack*) malloc(sizeof(Stack));
	stack->top = 0;
	stack->items = (int*) malloc (capacity*sizeof(int)+1);
	int i; for(i=0;i<capacity;i++) stack->items[i] = 0;
	stack->name = name;
	stack->capacity = capacity;
	return stack;
}
void stack_delete(Stack *s){
	free(s->items);
	free(s);
}
int stack_pop(Stack *s){
	int i = 0;
	while(i<s->capacity&&s->items[i]!=0) i++;
	i--;
	int item = s->items[i];
	s->items[i] = 0;
	i--;
	if(i!=-1) s->top = s->items[i];
	else s->top = 0;
	return item;
}
void stack_push(Stack *s, int item){
	if(s->items[0]==0){
		s->items[0] = item;
		s->top = item;
	} else{
		int i = 0;	
		while(s->items[i]!=0) i++;
		s->items[i] = item;
		s->top = item;
	}
}
bool stack_empty(Stack *s){
	if(s->items[0]==0) return 1; 
	else return 0;
}
