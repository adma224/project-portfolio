#include "bv.h"
#include <stdlib.h>
#include <stdio.h>

/*---------------------------------------
*    Title: Introduction to Bit Vectors Part 1 (beta)
*    Author: BlueberryTreeTutor
*    Date: August 2, 2011
*    Availability: Youtube.com
*----------------------------------------*/
BitVector *bv_create(uint32_t bit_len){
	BitVector* v = calloc(1,sizeof(BitVector*)+sizeof(uint32_t));
	v->vector = (uint8_t*) calloc(bit_len/8+1,8);
	v->length = bit_len;
	return v;
}
void bv_set_bit(BitVector *v, uint32_t i){
	v->vector[i/8] |= 1<<(i%8);
}
/*---------------------------------------*/
void bv_delete(BitVector *v){
	free(v->vector);
	free(v);
}
uint32_t bv_get_len(BitVector *v){
	return v->length;
}
void bv_clr_bit(BitVector *v, uint32_t i){
	if(bv_get_bit(v,i)==1)	v->vector[i/8] ^= 1<<(i%8);
}
uint8_t bv_get_bit(BitVector *v, uint32_t i){
	uint8_t is = v->vector[i/8] & 1<<(i%8);
	return is>>(i%8);
}
void bv_set_all_bits(BitVector *v){
	uint32_t c;
	for(c=0;c<(v->length/8);c++){
		v->vector[c] |= 0xff;
	}
}
