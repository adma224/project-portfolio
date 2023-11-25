EXEBINS = server client

DEPENDS = util $(EXEBINS)

OBJ 	= $(DEPENDS:%=%.o)
SRC 	= $(DEPENDS:%=%.cpp)
HDR 	= $(DEPENDS:%=%.h) unp.h

BINPATH = bin/
SRCPATH = src/

OBJECTS = $(OBJ:%=$(BINPATH)%)
SOURCES = $(SRC:%=$(SRCPATH)%)
HEADERS = $(HDR:%=$(SRCPATH)%)

CFLAGS	= -Wall -Wextra -Werror -Wunused
CC      = g++ -std=c++17 -lpthread -lreadline

MMCHECK = valgrind --leak-check=full --show-leak-kinds=all

PORTNUM = 3501
FILES   = forbidden-sites-file

ARGS    = $(PORTNUM) $(FILES)

all : $(EXEBINS)

$(EXEBINS) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $(BINPATH)$@ $(BINPATH)$@.o $(BINPATH)util.o

$(OBJECTS) : $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ -c $(@:$(BINPATH)%.o=$(SRCPATH)%.cpp)

clean :
	rm -f $(OBJECTS) $(EXEBINS:%=$(BINPATH)%)

srun :
	$(BINPATH)server_amurill9 $(PORTNUM) -@

crun : 
	bin/client 127.0.0.1 3500 amurill9 -@

check :
	$(MMCHECK) $(BINPATH)$(EXEBIN) $(ARGS)
