CXX = g++
CXXFLAGS = -Wall

OBJECTS = bst.o wordrange.o

treewrapper: $(OBJECTS)
	$(CXX) -g $(CXXFLAGS) -o wordrange bst.o wordrange.o

bst.o: bst.cpp bst.h
	$(CXX) -g $(CXXFLAGS) -c bst.cpp
	
treewrapper.o: bst.cpp bst.h wordrange.cpp
	$(CXX) -g $(CXXFLAGS) -c wordrange.cpp

check:
	valgrind --leak-check=full ./wordrange allwords-basic.txt simple-output.txt

clean: 
	rm -f *.o
	rm wordrange
