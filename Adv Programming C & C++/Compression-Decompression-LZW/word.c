#include "word.h"


Word *word_create(uint8_t *word, uint64_t word_len){
  Word* w = calloc(1,sizeof(word));
  w->word_len = word_len;
  w->word = calloc(word_len,sizeof(uint8_t));
  uint8_t i;
  for(i=0;i<word_len;i++) {
    w->word[i] = word[i];
  }
  return w;
}


void word_delete(Word *w){
  free(w->word);
  free(w);
}


WordTable *wt_create(){
  uint16_t i;
  WordTable* wt = calloc(1,sizeof(WordTable));
  uint8_t w[1];
  for(i=0;i<256;i++){
  	w[0] = (uint8_t)i;
  	wt->entries[i] = word_create(w,1);
  }
  return wt;
}


void wt_reset(WordTable *wt){
  int i;
  for(i=256;i<UINT16_MAX;i++){
  	word_delete(wt->entries[i]);
  	wt->entries[i] = NULL;
  }
}


void wt_delete(WordTable *wt){
  int i;
  for(i=0;i<UINT16_MAX;i++){
    if(wt->entries[i]){
      word_delete(wt->entries[i]);
      wt->entries[i] = NULL;
    }
  }
  free(wt);
}


void word_print(Word *w){
  uint64_t i;
  for(i=0;i<w->word_len;i++) printf("%c",w->word[i]);
}


void wt_print(WordTable *wt){
  uint64_t j;
  Word* w;
  for(j=1;wt->entries[j];j++){
    w = wt->entries[j];
  	uint64_t i;
  	for(i=0;i<w->word_len;i++) printf("%c",w->word[i]);
  	printf("\n");
  }
}


