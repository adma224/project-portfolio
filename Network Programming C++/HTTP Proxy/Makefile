EXEBIN  = proxy

OBJ 	= $(EXEBIN:%=%.o)
SRC 	= $(EXEBIN:%=%.cpp)
HDR 	= $(EXEBIN:%=%.h) unp.h

BINPATH = bin/
SRCPATH = src/

OBJECTS = $(OBJ:%=$(BINPATH)%)
SOURCES = $(SRC:%=$(SRCPATH)%)
HEADERS = $(HDR:%=$(SRCPATH)%)

CFLAGS	= -Wall -Wextra -Werror -Wunused
CC      = g++ -std=c++17 -lpthread 

MMCHECK = valgrind --leak-check=full --show-leak-kinds=all

PORTNUM = 3500
FILES   = forbidden-sites-file

ARGS    = $(PORTNUM) $(FILES)

all : $(EXEBIN)

$(EXEBIN) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $(BINPATH)$@ $(OBJECTS)

$(OBJECTS) : $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ -c $(SOURCES)

clean :
	rm -f $(OBJECTS) $(EXEBIN:%=$(BINPATH)%)

run :
	$(BINPATH)$(EXEBIN) $(ARGS)

check :
	$(MMCHECK) $(BINPATH)$(EXEBIN) $(ARGS)
