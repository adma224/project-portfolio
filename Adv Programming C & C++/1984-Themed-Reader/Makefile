SOURCE = newspeak.c ll.c hash.c bf.c speck.c bv.c lex.yy.c
OBJECTS = newspeak.o ll.o hash.o bf.o speck.o bv.o lex.yy.o
HEADERS = ll.h hash.h bf.h speck.h bv.h
CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
LFLAGS = -lfl
MEMCHECK = valgrind --leak-check=full
EXECUTABLES = newspeak

all : $(EXECUTABLES)

$(EXECUTABLES) : $(OBJECTS) $(HEADERS)
	$(CC) -o $(EXECUTABLES) $(OBJECTS) $(LFLAGS)

$(OBJECTS) : $(SOURCE) $(HEADERS)
	$(CC) -c $(SOURCE)

lex.yy.c: words.l
	flex words.l



clean :
	rm -f $(EXECUTABLES) $(OBJECTS) lex.yy.o lex.yy.c

check : 
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXECUTABLES) -b -s 10000000 -h 1000000

infer :
	infer run -- $(EXECUTABLES)
	