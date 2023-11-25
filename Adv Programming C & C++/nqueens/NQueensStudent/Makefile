EXEBIN=nqueens
SOURCE= main.cpp queens.cpp util.cpp
OBJECTS= main.o queens.o util.o
HEADERS= queens.h util.h
FLAG= -Wall -Wextra -Werror -Wpedantic -Wshadow
CXX=g++
FILES= simple-input.txt simple-output.txt

all: $(EXEBIN)

$(EXEBIN) : $(OBJECTS) $(HEADERS)
	$(CXX) -o $(EXEBIN) $(OBJECTS)

$(OBJECTS) : $(SOURCE) $(HEADERS)
	$(CXX) -c $(FLAGS) $(SOURCE)

clean:
	rm -f $(EXEBIN) $(OBJECTS)

check:
	valgrind --leak-check=full ./$(EXEBIN) $(FILES)

r:
	./$(EXEBIN) $(FILES)
