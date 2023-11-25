EXENAME = client server
EXEBINS = $(EXENAME:%=my%)

DEPENDS = util $(EXENAME:%=shell_%)

OBJ 	= $(DEPENDS:%=%.o)
SRC 	= $(DEPENDS:%=%.cpp)
HDR 	= $(DEPENDS:%=%.h) unp.h

BINPATH = bin/
SRCPATH = src/

OBJECTS = $(OBJ:%=$(BINPATH)%)
SOURCES = $(SRC:%=$(SRCPATH)%)
HEADERS = $(HDR:%=$(SRCPATH)%)

MMCHECK = valgrind --leak-check=full --show-leak-kinds=all

CFLAGS	= -Wall -Wextra -Werror -Wunused
CC      = g++ -std=c++17 -lreadline
PORT	= 3501

all : $(EXEBINS)

$(EXEBINS) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $(BINPATH)$@ $< $(BINPATH)shell_$(@:my%=%).o

$(OBJECTS) : $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ -c $(@:$(BINPATH)%.o=$(SRCPATH)%.cpp)

clean :
	rm -f $(OBJECTS) $(EXEBINS:%=$(BINPATH)%)

server :
	$(BINPATH)myserver $(PORT)

client :
	$(BINPATH)myclient 127.0.0.1 $(PORT)

check :
	$(MMCHECK) $(BINPATH)myserver $(PORT)
