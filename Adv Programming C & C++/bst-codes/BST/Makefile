CXX = g++
CXXFLAGS = -Wall

OBJECTS = bst.o treewrapper.o

treewrapper: $(OBJECTS)
	$(CXX) -g $(CXXFLAGS) -o treewrapper bst.o treewrapper.o

bst.o: bst.cpp bst.h
	$(CXX) -g $(CXXFLAGS) -c bst.cpp
	
treewrapper.o: bst.cpp bst.h treewrapper.cpp
	$(CXX) -g $(CXXFLAGS) -c treewrapper.cpp

clean: 
	rm -f *.o
	rm treewrapper
