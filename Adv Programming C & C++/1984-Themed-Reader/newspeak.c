#include "ll.h"
#include "hash.h"
#include "speck.h"
#include "bf.h"
#include <stdio.h>
#include <getopt.h>
#include <string.h>

extern FILE *yyin;
extern char *yytext;
extern int yylex(void);
extern int yylex_destroy(void);

int main(int argc, char** argv){
	
	bool sflag, bflag, mflag = false;
	int opt;
	uint32_t hash_size = 1;
	uint32_t bloom_size = 0x100000;
	while ((opt = getopt (argc , argv , "smb h: f: ") ) != -1){
		switch(opt){
			case 's' :
				sflag = true;
			break;
			case 'm' :
				if(bflag) return 1;
				mflag = true;
			break;
			case 'b' :
				if(mflag) return 1;
				bflag = true;
			break;
			case 'h' :
				hash_size = atoi(optarg);
			break;
			case 'f' :
				bloom_size = atoi(optarg);
			break;
		}
	}
	if(!mflag&&!bflag) return 1;

	BloomFilter* bloom = bf_create(bloom_size);
	HashTable* hash = ht_create(hash_size);

	yyin = fopen("badspeak.txt","r");
	while(yylex()!=EOF){
		bf_insert(bloom,yytext);
		ht_insert(hash,gs_create(yytext,NULL));
	}
	fclose(yyin);
	yyin = fopen("goodspeak.txt","r");
	while(yylex()!=EOF){
		char temp[strlen(yytext)];
		strcpy(temp,yytext);
		temp[strlen(temp)-1]=10;
		yylex();
		bf_insert(bloom,temp);
		ht_insert(hash,gs_create(temp,yytext));		
	}
	fclose(yyin);
	yyin = stdin;

	bool crime, change = false;

	ListNode* crimes = NULL;
	ListNode* changes = NULL;

	while(yylex()!=EOF){
		if(bf_probe(bloom,yytext)){
			ListNode* positive = ht_lookup(hash,yytext);
			if(positive){
				GoodSpeak* gs = positive->gs;
				if(!gs->new_speak){
					crime = true;
					if(!crime){
						crimes = ll_node_create(gs_create(gs->old_speak,gs->new_speak));
					} else{
						ll_insert(&crimes,gs_create(gs->old_speak,gs->new_speak));
					}
				} else{
					change = true;
					if(!change){
						changes = ll_node_create(gs_create(gs->old_speak,gs->new_speak));
					} else{
						ll_insert(&changes,gs_create(gs->old_speak,gs->new_speak));
					}
				}
			}
		}
	}

	if(crime){
		puts("\nDear Comrade ,\n\nYou have chosen to use degenerate words that may cause hurt\nfeelings or cause your comrades to think unpleasant thoughts.\nThis is doubleplus bad. To correct your wrongthink and\npreserve community consensus we will be sending you to joycamp\nadministered by Miniluv.\n\nYour errors :\n");
		ListNode* rider = crimes;
		while(rider!=NULL){
			printf("%s",rider->gs->old_speak);
			rider=rider->next;
		}
		if(change){
			printf("\nThink of these words on your vacation!\n\n");
			rider = changes;
			while(rider!=NULL){
				char old [strlen(rider->gs->old_speak)];
				char new [strlen(rider->gs->new_speak)];
				strcpy(old,rider->gs->old_speak);
				strcpy(new,rider->gs->new_speak);
				old[strlen(old)-1]=0;
				new[strlen(new)-1]=0;
				printf("%s -> %s\n",old,new);
				rider = rider->next;
			}
		}
	} else if(change){
		puts("\nDear Comrade,\n\nSubmitting your text helps to preserve feelings and prevent\nbadthink . Some of the words that you used are not goodspeak.\nThe list shows how to turn the oldspeak words into newspeak.\n");
			ListNode* rider = changes;
			while(rider!=NULL){
				char old [strlen(rider->gs->old_speak)];
				char new [strlen(rider->gs->new_speak)];
				strcpy(old,rider->gs->old_speak);
				strcpy(new,rider->gs->new_speak);
				old[strlen(old)-1]=0;
				new[strlen(new)-1]=0;
				printf("%s -> %s\n",old,new);
				rider = rider->next;
			}
	} else{
		puts("\nDear Comrade,\n\nThe Ministry of Truth has reviewed your text and has concluded\nthat no badspeak or oldspeak was used. Congratulations on being\nan outstanding citizen!\n");
	}

	printf("\n");

	if(crime) ll_delete(crimes);
	if(change) ll_delete(changes);
	
	yylex_destroy();

	bf_delete(bloom);
	ht_delete(hash);

	return 0;
}
