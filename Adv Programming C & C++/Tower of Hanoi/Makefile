
S = stack
FLAGS = -Wall -Wextra -Werror -Wpedantic
EXE = tower
OBJ = $(EXE).o $(S).o
SRC = $(S).c $(EXE).c

all : $(EXE)

$(EXE) : $(OBJ) $(S).h
	clang -o $(EXE) $(OBJ) -lm

$(OBJ) : $(SRC) $(S).h
	clang -c $(FLAGS) $(SRC)

clean :
	rm -f $(EXE) $(OBJ)

check : 
	valgrind --leak-check=full ./$(EXE) -s -r


