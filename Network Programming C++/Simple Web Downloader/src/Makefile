ADT_NAME  = client
SOURCES   = $(ADT_NAME).cpp
OBJECTS   = $(ADT_NAME).o
HEADERS   = $(ADT_NAME).h  
EXEBIN    = myweb
FLAGS     = -Wall -Wextra -Werror
EXERUN    = www.example.com 93.184.216.34:80/index.html
BIN       = ../bin/
SRC       = ../src/
INC 	  = -Iinc

all: $(EXEBIN)

$(EXEBIN) : $(OBJECTS) $(HEADERS)
	mv $(OBJECTS) $(BIN)
	g++ -o ../$(EXEBIN) $(BIN)$(OBJECTS)

$(BIN)$(OBJECTS) : $(SOURCES) $(HEADERS)
	g++ -c $(FLAGS) $(SOURCES) --outdir=$(BIN)

clean :
	rm -f ../$(EXEBIN) $(BIN)$(OBJECTS) ./output.dat

check:
	valgrind --leak-check=full --show-leak-kinds=all./$(EXEBIN) $(EXERUN)

run:
	../$(BIN)$(EXEBIN) $(EXERUN) -h
	../$(EXEBIN) $(EXERUN)
	cat output.dat
	rm output.dat

test1:
	valgrind --leak-check=full ../$(EXEBIN) not_a_website 93.184.216.34:80/index.html
test2:
	valgrind --leak-check=full ../$(EXEBIN) www.example.com 93.216.34:80/index.html
test3:
	valgrind --leak-check=full ../$(EXEBIN) www.example.com 93.184.216.34/index.html -h
test4:
	valgrind --leak-check=full ../$(EXEBIN) www.example.com 93.184.216.34:80 -h
