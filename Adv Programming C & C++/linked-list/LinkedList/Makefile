CXX = g++
CXXFLAGS = -Wall -std=c++11
SOURCE = test.cpp linkedlist.cpp listwrapper.cpp
OBJECTS = linkedlist.o test.o listwrapper.o
EADERS = linkedlist.h
CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
EXECUTABLES = test listwrapper

all : $(EXECUTABLES)

test: test.o linkedlist.o linkedlist.h
	$(CXX) -g $(CXXFLAGS) -o test test.o linkedlist.o

listwrapper: listwrapper.o linkedlist.o linkedlist.h
	$(CXX) -g $(CXXFLAGS) -o listwrapper listwrapper.o linkedlist.o 

$(OBJECTS) : $(SOURCE) $(HEADERS)
	$(CXX) -g $(CXXFLAGS) -c $(SOURCE)

clean:
	rm -f $(EXECUTABLES) $(OBJECTS)

check : 
	valgrind --leak-check=full --show-leak-kinds=all ./test
