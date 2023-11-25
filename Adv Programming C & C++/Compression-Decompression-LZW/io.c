#include "io.h"

uint64_t char_total = 0;
uint64_t bit_total = 0;

#define bytes  (bit_total/8)
static uint8_t char_buffer[BLOCK_SIZE];
static uint8_t code_buffer[BLOCK_SIZE];

void read_header(int infile, FileHeader *header){
  read(infile,&header->magic,4);
  lseek(infile,4,SEEK_CUR);
  read(infile,&header->file_size,8);
  read(infile,&header->protection,2);
  header->padding=0xffff;
  lseek(infile,4,SEEK_CUR);
}

void write_header(int outfile, FileHeader *header){
  write(outfile, &header->magic,4);
  uint32_t blank[1] = {0};
  write(outfile,blank,4);
  write(outfile,&header->file_size,8);
  write(outfile,&header->protection,2);
  write(outfile,&header->padding,2);
  write(outfile,&header->padding,2);
  write(outfile,&header->padding,2);
}

uint8_t next_char(int infile){
  if(char_total%BLOCK_SIZE==0){
    read(infile,char_buffer,BLOCK_SIZE);
  }
  return char_buffer[(char_total++)%BLOCK_SIZE];
}

void buffer_word(int outfile, Word *w){
  uint16_t curr;
  if(char_total%BLOCK_SIZE==0){
    flush_word(outfile);
    curr = 0;
  } else curr = char_total%BLOCK_SIZE;
  uint16_t i;
  for(i=0;i<w->word_len;i++){
    char_buffer[curr] = w->word[i];
    char_total++;
    if(char_total%BLOCK_SIZE==0){
      flush_word(outfile);
      curr = 0;
    } else curr++;
  }
  char_total++;
}

void flush_word(int outfile){
  write(outfile,char_buffer,char_total%BLOCK_SIZE);
  int i;
  for(i=0;i<BLOCK_SIZE;i++){
    char_buffer[i] = 0;
  }
}

void buffer_code(int outfile, uint16_t code, uint8_t bit_len){
  uint16_t curr;
  if(bytes%BLOCK_SIZE==0&&bit_total){
    flush_code(outfile);
    curr=0;
  } else curr = (bit_total/8)%BLOCK_SIZE;
  uint16_t hex = code;
  int shift = (int) 8-(bit_total%8+bit_len);
  if(shift>=0){
    hex<<=shift;
    code_buffer[curr]|=hex;
    bit_total+=bit_len;
  } else{
    int unshift = 8+shift;
    shift = abs(shift);
    uint16_t unhex = hex;
    hex>>=shift;
    code_buffer[curr]|=hex;
    bit_total+=(bit_len-shift);
    if(bytes%BLOCK_SIZE==0&&bit_total){
      flush_code(outfile);
      curr=0;
    } else curr++;
    unhex<<=unshift;
    code_buffer[curr]|=unhex;
    bit_total+=shift;
  }
}

void flush_code(int outfile){
  
  if(bytes%BLOCK_SIZE==0) {
    write(outfile,code_buffer,BLOCK_SIZE);
  }
  else {
    write(outfile,code_buffer,bytes%BLOCK_SIZE);
  }
  int i;
  for(i=0;i<BLOCK_SIZE;i++){
    code_buffer[i] = 0;
  }
}

uint16_t next_code(int infile, uint8_t bit_len){
  uint16_t curr;
  if(bytes%BLOCK_SIZE==0){
    read(infile,code_buffer,BLOCK_SIZE);
    curr = 0;
  }
  else curr = bytes%BLOCK_SIZE;
  int lshift = (int) bit_total&8;
  int rshift = (int) 8-(bit_total%8+bit_len);

  uint8_t clean = 0;
  clean = code_buffer[curr] << lshift;
  uint16_t code = clean>>lshift;

  if(rshift<0){
    bit_total += (8-lshift);
    if(bytes%BLOCK_SIZE==0){
      read(infile,code_buffer,BLOCK_SIZE);
      curr=0;
    } else curr++;
    code <<= abs(rshift);
    uint8_t temp = code_buffer[curr]>>(8-abs(rshift));
    code |= temp;
    bit_total += abs(rshift);
  } else{
    bit_total+=bit_len;
    code >>= rshift;
  }
  return code;
}

