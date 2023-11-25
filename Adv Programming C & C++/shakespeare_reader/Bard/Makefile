SOURCE = william.cpp linkedList.cpp
OBJECTS = william.o linkedList.o
HEADERS = linkedList.h
CC = g++
CFLAGS = -Wall -Wextra -Werror -Wpedantic
MEMCHECK = valgrind --leak-check=full
EXECUTABLES = bard



all : $(EXECUTABLES)

$(EXECUTABLES) : $(OBJECTS) $(HEADERS)
	$(CC) -g $(CFLAGS) -o  $(EXECUTABLES) $(OBJECTS) 

$(OBJECTS) : $(SOURCE) $(HEADERS)
	$(CC) -g $(CFLAGS) -c $(SOURCE) 

clean :
	rm -f $(EXECUTABLES) $(OBJECTS)

check : 
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXECUTABLES) simple-input.txt simple-output.txt

make r:
	./$(EXECUTABLES) simple-input.txt simple-output.txt

