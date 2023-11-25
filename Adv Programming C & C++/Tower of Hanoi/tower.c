#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "stack.h"
#include <string.h>
#include <math.h>

static int r = 0;
static int s = 0;
void recursion(int i, int* a, int* to, int* and, char A, char B, char C);
void stack(int n, Stack* a, Stack* b, Stack* c);

int main(int argc, char** argv){
	int opt;
	int input = 0; 
	while((opt=getopt(argc,argv,"nsr :"))!=-1){
		switch(opt){
			case 'n' :
				input = 1;
			break;
			case '?' :
				
			break;
			case 's' :
				s=1;
			break;
			case 'r' :
				r=1;
			break;
		}
	}
	int n;
	if(input==1) n=atoi(argv[optind]); 
	else n = 5;

	if(s == 1){
		printf("================================\n---------- STACKS ----------\n================================\n");
		Stack* a = stack_create(n,'A');
		Stack* b = stack_create(n,'B');
		Stack* c = stack_create(n,'C');

		int i;
		for(i=n;i>0;i--) stack_push(a,i);

		if(n%2==0) stack(n,a,b,c);
		else stack(n,a,c,b);

		stack_delete(a);
		stack_delete(b);
		stack_delete(c);

		printf("\nNumber of moves: %d\n\n",s);
	}
	if(r == 1) {
		printf("================================\n--------  RECURSION  --------\n================================\n");
		r--;
		int* a = malloc(sizeof(int));
		int* b = malloc(sizeof(int));
		int* c = malloc(sizeof(int));
		*a=n;
		*b=0;
		*c=0;
		char A = 'A';
		char B = 'B';
		char C = 'C';
		if(n%2==0) recursion(n,a,b,c,A,B,C);
		else recursion(n,a,c,b,A,C,B);
		free(a);
		free(b);
		free(c);
		printf("\nNumber of moves: %d\n\n",r);
	}
	return 0;
}
void recursion(int i, int* a, int* b, int* c, char A, char B, char C){
	if(i==1){
		printf("Move disk %d from peg %c to peg %c\n", i, A, C);
		r++;
	} else{
		recursion(i-1, a, c, b, A, C, B);
		printf("Move disk %d from peg %c to peg %c\n", i, A, C);
		r++;
		recursion(i-1, b, a, c, B, A, C);
	}
}
void stack(int n, Stack* a, Stack* b, Stack* c){
	s--;
	int total = pow(2, n)-1;
	int i;
	for(i=1;i<=total;i++){
		if(i%3==1){
			if(stack_empty(c)||(a->top<c->top&&!stack_empty(a))){
				printf("Move disk %d from peg %c to peg %c\n", a->top, a->name, c->name); 
				stack_push(c,stack_pop(a));
			} else {
				printf("Move disk %d from peg %c to peg %c\n", c->top, c->name, a->name); 
				stack_push(a,stack_pop(c));
			}
		}
		if(i%3==2){
			if(stack_empty(b)||(a->top<b->top&&!stack_empty(a))){
				printf("Move disk %d from peg %c to peg %c\n", a->top, a->name, b->name); 
				stack_push(b,stack_pop(a));
			} else {
				printf("Move disk %d from peg %c to peg %c\n", b->top, b->name, a->name); 
				stack_push(a,stack_pop(b));
			}
		}
		if(i%3==0){
			if(stack_empty(c)||(b->top<c->top&&!stack_empty(b))){
				printf("Move disk %d from peg %c to peg %c\n", b->top, b->name, c->name); 
				stack_push(c,stack_pop(b));
			} else {
				printf("Move disk %d from peg %c to peg %c\n", c->top, c->name, b->name); 
				stack_push(b,stack_pop(c));
			}
		}
		s++;
	}
}
