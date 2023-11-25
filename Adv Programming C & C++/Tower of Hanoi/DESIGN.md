DESIGN

The design behind this code can be broken down in four parts: The recursive funtion,
the stacks function, the user input and the calls for recursion and stacks functions.

Recursion: For this just do a recursive funtion where the number of the biggest disk to
move from a stack is inputed as an argument. The recursive logic behind the algorith is
by cosidering each movement of disks a new stack, from size 1 to n where n is the largest
disk. Design an algorithm that sends stacks around to there corresponding places, and each
time you want to move a stack call the algorith again for a stack with a smaller bottom 
disk number. Finally, create a case where if the stack's bottom disk is 1, to just move it 
to the corresponding peg, this would be the base case.

Stack: for this algorithm a bit more thinking is needed to be made. First of all notice 
that the game cycles interactions between pegs in the following way: a with b, a with c, 
c with b. Sometimes in an interaction between a with b can be top of a moves to b, but other
times it would be top of b moves to a. For this just analyze if a move is a possible in 
an interaction and if not then perform the opposite option. For example if the interaction
is between a with b, then see if top of a can go to the top of b, if not, move from b to a.
Do this a number of times equal to 2^n-1 where n is the largest disk in the initial stack.

User Input: Use getopt() funtion to receive the user input and create three cases: -s for
stack function, -r for recursion function, and -n for the number input. Use optind after 
the getopt() cycle is over to get the last element of argv which is the number of disks, 
else set default to 5.

Calls: for calling the recursive function allocate int pointers and plug them in into the 
function, as well as their respective char names. For calling stacks create 3 stacks and 
fill one up to n, then call it for the three stacks. Remember to free your int pointers
and stacks when done using them. For both of these cases remember to switch the target 
stack and the auxiliary stack if n is even.
