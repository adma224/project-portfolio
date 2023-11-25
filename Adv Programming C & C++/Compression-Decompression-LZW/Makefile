SOURCE  = io.c trie.c word.c main.c
OBJECTS  = io.o trie.o word.o main.o
HEADERS  = io.h trie.h word.h endian.h
CC  = clang
CFLAGS  = #-Wall -Wextra -Werror -Wpedantic
MEMCHECK  = valgrind --leak-check=full
EXECUTABLES = lzwcoder

all : $(EXECUTABLES)

$(EXECUTABLES) : $(OBJECTS) $(HEADERS)
	$(CC) -o $(EXECUTABLES) $(OBJECTS) -lm
	ln ./$(EXECUTABLES) encode
	ln ./$(EXECUTABLES) decode

$(OBJECTS) : $(SOURCE) $(HEADERS)
	rm -f encode decode .lzwTest
	$(CC) -c $(CFLAGS) $(SOURCE)

clean:
	rm -f $(EXECUTABLES) $(OBJECTS) encode decode 

check: 
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXECUTABLES) -v -d -i poem.lzw -o poem.txt 


