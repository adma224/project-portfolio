ADT_NAME  = client
SOURCES   = $(ADT_NAME).cpp
OBJECTS   = $(ADT_NAME).o
HEADERS   = $(ADT_NAME).h  
EXEBIN    = myclient
EXERUN    = 3500
FLAGS     = -Wall -Wextra -Werror

all: myserver myclient

myserver : server.o server.h
	g++ -o myserver -std=c++17 server.o

myclient : client.o client.h
	g++ -o myclient -std=c++17 client.o

$(OBJECTS) : $(SOURCES) $(HEADERS)
	g++ -c $(FLAGS) $(SOURCES) 

clean :
	rm -f $(EXEBIN) $(OBJECTS) 

check:
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXEBIN) $(EXERUN)

run:
	./$(EXEBIN) $(EXERUN)
