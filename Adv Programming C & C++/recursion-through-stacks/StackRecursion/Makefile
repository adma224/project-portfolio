CXX = g++
CXXFLAGS = -Wall -std=c++11

OBJECTS = node.o list.o stackrecursion.o

stackrecursion: $(OBJECTS)
	$(CXX) -g $(CXXFLAGS) -o $@ $^
check:
	valgrind --leak-check=full ./stackrecursion in.txt out.txt
clean: 
	rm -f *.o stackrecursion
$(OBJECTS): node.h list.h
