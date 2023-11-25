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
//    FindPath.c
// File Description:
//    Main code for Graph ADT that calculates BFSs of input file
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
      exit(1);
   }
   if( out==NULL ){
      //printf("Unable to open file %s for writing\n", argv[2]);
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
      addEdge(G,a,b);
   }

    printGraph(out, G);
   
   while(fgets(line, MAX_LEN, in) != NULL){
      count++;
      token = strtok(line," \n");
      if(atoi(token)==0){
         break;
      }
      a = atoi(token);
      tokenlist[0] = '\0';
      strcat(tokenlist, token);
      token = strtok(NULL, " \n");
      b = atoi(token);
      BFS(G, a);
      fprintf(out,"\nThe distance from %i to %i is ",G->source,b);
      if(G->distance[b]==INF){
         fprintf(out, "infinity\n");
         fprintf(out, "No %i-%i path exists\n",a,b);
      }else{
         fprintf(out,"%i\nA shortest %i-%i path is:",G->distance[b],a,b);
         List path = newList();
         getPath(path,G,b);
         printList(out,path);
         freeList(&path);
         fprintf(out,"\n");
      }
      

   }
   
   freeGraph(&G);
   fclose(out);
   fclose(in);

   return(0);
}
