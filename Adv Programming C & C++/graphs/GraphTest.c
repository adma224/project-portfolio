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
//    GraphTest.c
// File Description:
//    Test main code for Graph ADT
//-----------------------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include"Graph.h"

int main(int argc, char* argv[]){
   int n=15;
   Graph G;
   G = newGraph(n);
   int i;
   int j;
   for(i=1;i<=n;i++){
      for(j=1;j<=n;j++){
         if((i-j)>0&&((i+j)==n||n%j==(i/j))){
            addEdge(G,i,j);
         }
      }
   }
   printGraph(stdout,G);
   printf("\nSize %i\n", getSize(G));
   printf("Order %i\n", getOrder(G));
   int a=1; int b=6;
   BFS(G,a);

   List l = newList();
   
   fprintf(stdout,"\nThe distance from %i to %i is %i",G->source,b,G->distance[b]);
   fprintf(stdout,"\nA shortest %i-%i path is: ",a,b);
   

   getPath(l,G,b);
   printList(stdout,l);
   printf("\n");
   freeList(&l);
   freeGraph(&G);
   return(0);
}