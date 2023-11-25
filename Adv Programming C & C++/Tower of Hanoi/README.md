README for Tower of Hanoi ADT

Tower of Hanoi is an ancient game where 3 wooden pegs are used to hold 
wooden disks in a stack, where the disks are each bigger from the one
above. The goal is to move a whole stack from one peg to another only by 
moving the top disk only to empty pegs or on top of a bigger disk.

This ADT provides a solution to this game, where the correct list of moves
is displayed to win the game in the least amount of moves for any number of
disks on the starting stack. 

How to use (through the command line)

	make - compiles the code and creates excecutables
	./tower - runs code, follow with one of the next options
		-r - runs the code with a recursive algorith
		-s - runs the code using an iterative algorith using stacks
		-n (#) - sets number of disks, default to 5

