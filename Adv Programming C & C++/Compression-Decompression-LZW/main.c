#include "io.h"
#include "trie.h"
#include "word.h"
#include "endian.h"

#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

extern uint64_t char_total;
extern uint64_t bit_total;

void print_usage(){
  fprintf(stderr,"Usage: <exe> | -v | -c | -d | -i <infile> | -o <outfile>\n");
  fprintf(stderr,"    Set <exe> to: ./lzwcoder | encode | decode\n");
  exit(2);
}
void exit_error(char* string){
  fprintf(stderr,"Error: %s\n\n - Exit Failure\n",string);
  exit(3);
}

int main(int argc, char** argv){

  char* infile;
  char* outfile;
  int in, out;
  bool vflag, cflag, dflag, iflag, oflag = false;
  int opt;
  while ((opt = getopt (argc , argv , "vcd i: o: ") ) != -1){
    switch(opt){
      case 'v' :
        vflag = true;
      break;
      case 'c' :
        if(dflag) print_usage();
        cflag = true;
      break;
      case 'd' :
        if(cflag) print_usage();
        dflag = true;
      break;
      case 'i' :
        iflag = true;
        infile = optarg;
      break;
      case 'o' :
        oflag = true;
        outfile = optarg;
      break;
      case '?' :
        print_usage();
      break;
    }
  }
  if(strcmp(argv[0],"./lzwcoder")==0||strcmp(argv[0],"encode")==0||strcmp(argv[0],"decode")==0){
    
    if(iflag) in = open(infile,O_RDONLY);
    else in = 0;
    if(oflag) out = open(outfile,O_WRONLY|O_CREAT);
    else out = 1;
    int longest=0;
    int traverse=0;
    FileHeader* header = calloc(1,sizeof(FileHeader*));
    if(cflag||strcmp(argv[0],"encode")==0){

      uint64_t size = lseek(in, 0, SEEK_END);
      header->file_size = size;
      header->magic = 0xdeadcafe;
      header->padding = 0;
      header->protection = 0xa481;
      write_header(out,header);
      TrieNode* trie = trie_create();
      TrieNode* curr_node = trie;
      TrieNode* next_node = NULL;
      uint16_t next_avail_code = 256;
      uint8_t curr_char = 0;

      lseek(in,0,SEEK_SET);
      curr_char = next_char(in);
      while(char_total!=(size+1)){
        next_node = trie_step(curr_node,curr_char);
        traverse++;
        if(char_total==0||next_node){
          curr_node=next_node;
        } else{
          uint8_t bit_len = log2(next_avail_code)+1;
          buffer_code(out,curr_node->code,bit_len);
          traverse++;
          curr_node->children[curr_char] = trie_node_create(next_avail_code);
          next_node = curr_node->children[curr_char];
          curr_node = trie->children[curr_char];
          next_avail_code++;
          if(traverse>longest) longest=traverse;
          traverse=0;
        }
        if(next_avail_code==UINT16_MAX){
          trie_reset(trie);
          curr_node = trie->children[curr_char];
          next_avail_code=256;
        }
        curr_char = next_char(in);
      }
      buffer_code(out,curr_node->code,log2(next_avail_code)+1);
      flush_code(out);
      trie_delete(trie);

      if(vflag){
        printf("Original file size: %lu bytes\n",size);
        printf("Compressed file size: %lu bytes\n",bit_total/8);
        uint64_t byte_total = bit_total/8;
        float ratio = ((float)byte_total/(float)size)*100;
        printf("Compression Ratio %0.4f\n",ratio);
        printf("Longest word length: %i\n",longest);
        
      }
    } else if(dflag||strcmp(argv[0],"decode")){

      read_header(in,header);
      if(header->magic!=MAGIC) {
        if(iflag) close(in);
        if(oflag) close(out);
        free(header);
        exit_error("File not compatible <infile> - Please choose lzw file"); 
      }
      WordTable* table = wt_create();

      uint16_t next_avail_code=256;
      bool rflag = false;
      uint64_t size = lseek(in, 0, SEEK_END);
      lseek(in, 0, SEEK_SET);
      uint8_t bit_len;

      uint16_t curr_code;
      uint16_t prev_code;

      Word* curr_entry;
      Word* prev_entry;
      Word* missing_entry;

      uint8_t* curr_word;
      uint8_t* prev_word;
      int j;
      
      while(char_total!=size){

        bit_len=log2(next_avail_code+1)+1;
        curr_code = next_code(in,bit_len);
        curr_entry = table->entries[curr_code];
        char_total++;
        if(char_total==0||rflag){
          buffer_word(out,curr_entry);
          prev_word = curr_word;
          rflag = false;
        } else if(curr_entry!=NULL){
          curr_word = curr_entry->word;
          prev_entry = table->entries[prev_code];/*
          prev_word = prev_entry->word;
          uint8_t new_word[prev_entry->word_len+1];
          for(j=0;j<prev_entry->word_len;j++) new_word[j] = prev_word[j];
          new_word[prev_entry->word_len] = curr_word[0];
          table->entries[next_avail_code] = word_create(new_word,prev_entry->word_len+1);
          next_avail_code++;
          buffer_word(out,curr_entry);
        } else{
          prev_entry = table->entries[prev_code];
          prev_word = prev_entry->word;

          uint8_t new_word[prev_entry->word_len+1];
          for(j=0;j<prev_entry->word_len;j++) new_word[j] = prev_word[j];
          new_word[prev_entry->word_len] = prev_word[0];

          missing_entry = word_create(new_word,prev_entry->word_len+1);
          table->entries[next_avail_code] = missing_entry;
          next_avail_code ++;
          buffer_word(out,missing_entry);*/
        }

        prev_code = curr_code;
        if(next_avail_code==UINT16_MAX-1){
          wt_reset(table);
          rflag = true;
          next_avail_code = 256;
        }
      }
      wt_delete(table);
    }
    
    if(iflag) close(in);
    if(oflag) close(out);
    free(header);

  } else{
    print_usage();
  }
  return 0;
}
