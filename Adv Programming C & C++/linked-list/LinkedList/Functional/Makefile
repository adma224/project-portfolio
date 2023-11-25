CXX = g++
CXXFLAGS = -Wall -std=c++11

OBJECTS = linkedlistfunc.o listwrapper.o

listwrapper: $(OBJECTS)
	$(CXX) -g $(CXXFLAGS) -o listwrapper linkedlistfunc.o listwrapper.o

linkedlistfunc.o: linkedlistfunc.cpp linkedlistfunc.h
	$(CXX) -g $(CXXFLAGS) -c linkedlistfunc.cpp
	
listwrapper.o: linkedlistfunc.cpp linkedlistfunc.h listwrapper.cpp
	$(CXX) -g $(CXXFLAGS) -c listwrapper.cpp

clean: 
	rm -f *.o
	rm listwrapper
