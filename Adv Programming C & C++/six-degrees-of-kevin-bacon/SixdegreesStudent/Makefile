WARNING   = -Wall -Wextra -Wpedantic -Wshadow -Wold-style-cast
CXX       = g++ -std=c++11 -g -O0
CXXWARN   = ${CXX} ${WARNING} -fdiagnostics-color=never

MODULES   = graph util relater
CPPSRC    = ${MODULES:=.cpp} # main.cpp
HDRSRC    = ${MODULES:=.h}
OBJECTS   = ${CPPSRC:.cpp=.o}
EXECBIN   = sixdegrees
MODSRC    = ${foreach MOD, ${MODULES}, ${MOD}.h ${MOD}.cpp}
MISCSRC   = ${filter-out ${MODSRC}, ${HDRSRC} ${CPPSRC}}
FILES     = simple-input.txt simple-output.txt

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${CXXWARN} -o${EXECBIN} ${OBJECTS}

clean:
	-rm -f ${OBJECTS} ${EXECBIN}
	
check:
	valgrind --leak-check=full ./${EXECBIN} ${FILES}
