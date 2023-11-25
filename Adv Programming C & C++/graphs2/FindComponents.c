//-----------------------------------------------------------------------------------------
// Author: 
//    Adrian Murillo
// CruzID: 
//    amurill9
// ID#: 
//    1573260
// Institution:
//    UC Santa Cruz
// Course:
//    CMPS 101
// File Name:
//    FindComponents.c
// File Description:
//    Main code for Graph ADT that calculates DFSs of input file
//-----------------------------------------------------------------------------------------
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "Graph.h"

#define MAX_LEN 160

typedef char string[MAX_LEN];

int main(int argc, char * argv[]){

   int count=0;
   FILE *in, *out;
   string line;
   string tokenlist;
   char* token;
   int a,b;

   // check command line for correct number of arguments
   if( argc != 3 ){
      //printf("Usage: %s <input file> <output file>\n", argv[0]);
      exit(1);
   }

   // open files for reading and writing 
   in = fopen(argv[1], "r");
   out = fopen(argv[2], "w");
   if( in==NULL ){
      //printf("Unable to open file %s for reading\n", argv[1]);
      fclose(in);
      fclose(out);
      exit(1);
   }
   if( out==NULL ){
      //printf("Unable to open file %s for writing\n", argv[2]);
      fclose(out);
      fclose(in);
      exit(1);
   }
   fgets(line, MAX_LEN, in);
   token = strtok(line, " \n");
   Graph G = newGraph(atoi(token));
   //printf("token %i\n", getOrder(G));
   while( fgets(line, MAX_LEN, in) != NULL)  {
      count++;
      token = strtok(line, " \n");
      if(atoi(token)==0){
         break;
      }
      //printf("Token: %s ",token);
      a = atoi(token);
      tokenlist[0] = '\0';
      strcat(tokenlist, token);
      token = strtok(NULL, " \n");
      //printf("Token: %s",token);
      //printf("\n");
      b = atoi(token);
      addArc(G,a,b);
   }
   List L = newList();
   int i;
   for(i=1; i<=getOrder(G); i++) append(L, i);
   fprintf(out,"Adjacency list representation of G:\n");
   printGraph(out, G);
   DFS(G,L);
   Graph T1 = transpose(G);
   Graph T2 = transpose(G);
   List S = newList();
   int k=1;
   clear(S);
   while(length(L)>0){
      append(S,front(L));
      for(i=2; i<=getOrder(T1); i++) append(S, 0);
      DFS(T1,S);
      while(length(S)>0){
         for(moveFront(L);index(L)!=-1;moveNext(L)){
            if(front(S)==get(L)){
               deleteFront(S);
               delete(L);
               moveFront(L);
            }
         }
      }
      k++;
   }
   fprintf(out,"\nG contains %i strongly connected components:\n",k-1);
   clear(L);
   for(i=1; i<=getOrder(G); i++) append(L, i);
   DFS(G,L);
   k=1;
   clear(S);
   while(length(L)>0){
      append(S,front(L));
      for(i=2; i<=getOrder(T2); i++) append(S, 0);
      DFS(T2,S);
      fprintf(out,"Component %i: ",k);
      printList(out, S);
      fprintf(out,"\n");
      while(length(S)>0){
         for(moveFront(L);index(L)!=-1;moveNext(L)){
            if(front(S)==get(L)){
               deleteFront(S);
               delete(L); 
               moveFront(L);
            }
         }
      }
      k++;
   }
   freeGraph(&G);
   freeGraph(&T1);
   freeGraph(&T2);
   fclose(out);
   fclose(in);
   freeList(&L);
   freeList(&S);
   return(0);
}
