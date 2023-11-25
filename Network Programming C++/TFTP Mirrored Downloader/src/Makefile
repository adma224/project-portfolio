ADTNAME   = tftp_
SOURCES   = tftp_client.cpp tftp_server.cpp util.cpp
OBJECTS   = tftp_client.o tftp_server.o util.o
HEADERS   = tftp_client.h tftp_server.h util.h unp.h
EXEBIN    = myserver myclient
FLAGS     = -Wall -Wextra -std=c++17 -lpthread 
EXERUN    = www.example.com 93.184.216.34:80/index.html

all: $(EXEBIN)
	mv *.o ../bin/
	mv $(EXEBIN) ../

myclient : tftp_client.o util.o tftp_client.h util.h unp.h
	g++ -std=c++17 -lpthread -o myclient tftp_client.o util.o

myserver : tftp_server.o util.o tftp_server.h util.h unp.h
	g++ -std=c++17 -lpthread -o myserver tftp_server.o util.o

$(OBJECTS) : $(SOURCES) $(HEADERS)
	g++ -c $(FLAGS) $(SOURCES) 

clean :
	rm -f $(EXEBIN) $(OBJECTS) 

check:
	valgrind --leak-check=full --show-leak-kinds=all ./myserver 69

server:
	../myserver 3500

client: 
	./myclient ./sample_servers.txt 22 ./output.txt ./sample_output.txt 
