#include "bf.h"

BloomFilter *bf_create(uint32_t size) {
  BloomFilter *bf = (BloomFilter *)malloc(sizeof(BloomFilter));
  if (bf) {
    bf->size = size;
    bf->primary[0] = 0xfc28ca6885711cf7; // U.S. Consitution
    bf->primary[1] = 0x2841af568222f773;
    bf->secondary[0] = 0x85ae998311115ae3; // Il nome della rosa
    bf->secondary[1] = 0xb6fac2ae33a40089;
    bf->filter=bv_create(size);
    return bf ;
  }
  return (BloomFilter *)NIL;
}

void bf_delete(BloomFilter *bf){
  bv_delete(bf->filter);
  free(bf);
}

void bf_insert(BloomFilter *bf , char *key){
  uint32_t h1 = hash(bf->primary, key);
  uint32_t h2 = hash(bf->secondary, key);
  bv_set_bit(bf->filter, h1%bf->size);
  bv_set_bit(bf->filter, h2%bf->size);
}

bool bf_probe(BloomFilter *bf, char *key){
  uint32_t h1 = hash(bf->primary, key);
  uint32_t h2 = hash(bf->secondary, key);
  if(bv_get_bit(bf->filter, h1%bf->size)==1&&bv_get_bit(bf->filter, h2%bf->size)==1) return true;
  else return false;
}

