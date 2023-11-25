//-----------------------------------------------------------------------------
// FileIO.c
// Illustrates file input-output commands and text processing using the
// string functions strtok and strcat.
//-----------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "List.h"

#define MAX_LEN 160

typedef char string[MAX_LEN];


int main(int argc, char * argv[]){

   int count=0;
   FILE *in, *out;
   string line;
   string tokenlist;
   char* token;
   string lines[10240];

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
   int i=0;
   while( fgets(line, MAX_LEN, in) != NULL)  {
      count++;
      token = strtok(line, " \n");
      tokenlist[0] = '\0';
      if(token!=NULL){
         strcat(tokenlist, token);
         token = strtok(NULL, " \n");
      }
      while( token!=NULL ){
         strcat(tokenlist, " ");
         strcat(tokenlist, token);
         //strcat(tokenlist, "\n");
         token = strtok(NULL, " \n");
      }
      strcpy(lines[i],tokenlist);
      //printf("Token: %s, tokenlist: %s, line:%s, count:%i\n",token,tokenlist,line,count);
      //fprintf(stderr, "tokenlist:%s  lines[%i]:%s count:%i  \n",tokenlist,i, lines[i], count);
      i++;
   }
   /*
   for(int j=0;j<count;j++){
      fprintf(stdout, "%s\n", lines[0]);
   }*/
   //printf("\n");
   
   List list = newList();
   append(list, 0);
   moveFront(list);

   for(int k=1; k<count; k++){
      int high =1;
      string str1;
      string str2;
      strcpy(str1, lines[k]);
      strcpy(str2, lines[get(list)]);
      //fprintf(stdout,"Finding spot for str1: %s\n", str1);
      moveFront(list);
      if(index(list)==(length(list)-1)){
         //printf("Comparing strings %s and %s\n", str1, str2);
         if(strcmp(str1,str2)<0){
            high=0;
         }
      }else{
         while(high==1&&index(list)<(length(list)-1)){
            strcpy(str2,lines[get(list)]);
            //printf("Comparing strings %s and %s\n", str1, str2);
            if(strcmp(str1,str2)<=0){
               //printf("Found place!  ");
               high=0;
               break;
            }
            moveNext(list);
         }
         if(index(list)==(length(list)-1)){
            strcpy(str2,lines[get(list)]);
            //printf("Comparing strings %s and %s\n", str1, str2);
            if(strcmp(str1,str2)<0){
               high=0;
            }
         }
      }
      
      if(high==1){
         append(list, k);
         //printf("Inserted at the end\n");
      }else{
         insertBefore(list, k);
         //printf("Inserted at position %i\n", (index(list)-1));
      }
      
   }
   
   for(moveFront(list);index(list)>=0;moveNext(list)){
      fprintf(out, "%s\n", lines[get(list)]);
   }
   
   freeList(&list);
   
   fclose(out);
   fclose(in);

   return(0);
}
