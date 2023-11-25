CXX = g++
CXXFLAGS = -Wall

OBJECTS = binheap.o heapwrapper.o

heapwrapper: $(OBJECTS)
	$(CXX) -g $(CXXFLAGS) -o heapwrapper binheap.o heapwrapper.o

binheap.o: binheap.cpp binheap.h
	$(CXX) -g $(CXXFLAGS) -c binheap.cpp
	
heapwrapper.o: binheap.cpp binheap.h heapwrapper.cpp
	$(CXX) -g $(CXXFLAGS) -c heapwrapper.cpp

clean: 
	rm -f *.o
	rm heapwrapper
