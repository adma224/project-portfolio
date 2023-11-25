### HW2: Place your queens with care

This assignment is on solving a classic backtracking problem without using recursion. The main CS objective
is to understand how to use stacks to simulate recursion.

### Assignment details

Get a pdf of the assignment description from https://users.soe.ucsc.edu/~sesh/Teaching/2020/CSE101/HW/nqueens.pdf

Here are the relevant files and directories.

**NQueens/**: This folder contains the files for your submission. You must write your code in this folder. Your Makefile
must be in this directory, and should create the executable "nqueens" in this directory. Here is a description of files
within this directory.

* simple-input.txt, simple-output.txt: These are test input files, to check your code. Please leave these files in this directory.
The checking script with check whether your output (for simple-input.txt) matches this output.
* Tests/: This directory has a number of test input and corresponding output files. These files have more varied inputs.
I stress, these tests do not cover all cases. Passing these test cases does not mean your answer is correct, though
it should catch many bugs. The Tests/ folder contains an extra copy of simple-input.txt and simple-output.txt, if you accidentally overwrite them.
* Helper/PrintSolutionHelper.java: This is a piece of Java code that prints your solution onto the console. This is very helpful in debugging code. 
You can read instructions to run in the file itself as well. Compile it by running: "javac PrintSolutionHelper.java" (in the directory Helper/). 
This should generate a class file, which you can then run as "java PrintSolutionHelper <SOLUTION FILE>". The <SOLUTION FILE> is the text file that your
code generates as output. PrintSolutionHelper will print out your solutions as actual chessboards on the console.

**Do not delete any of files in the Tests directory!**

**checkcode.sh**: *This is very important!* This is a script to do some basic checks on your solution. You can run it as "./checkcode.sh", in terminal (while you're in the workspace folder). I recommend that you keep a separate terminal open, where you only run this command.  The command produces console output that explains the checks. If it finally ends with "PASS", this means your solution passed the tests. This does not guarantee that your code is correct, or that you will get any points. But if your code does not pass the test, you will not get any points when you submit. To ensure that the scripts runs properly, it is best to delete your executable and object files (*.o) files from the Bard/ directory. This ensures that this script will regenerate the executable, to check for any errors in compiling. Ideally, you should define a "clean" rule, so that the command "make clean" deletes the object and executable files.

**README.md**: That is the file you're reading right now :) You can click on this file, to get a tab with this. Again, keep this open for your convenience.

**RoboGrader/**: This has my checking and testing scripts. Do not modify anything here!

### Coding

Ideally, you should code directly through the command line, using an editor like vi or vim. Alternately, you can upload files directly. Go to the File menu, and click on "Upload".  But pleae compile your codes through the command line, and make sure everything runs in the Codio box. There is no guarantee that code compiled on your machine works on the Codio box (often, because of incompatible OSes or text editors introducing strange symbols, etc.) We will be testing your code directly in the Codio box, so ensure that you compile codes here.  Please use the checkcode.sh script to ensure everything is running. A significant part of grading is automated, so it is essential that you follow these rules.
  
Even if you're not familiar with Unix, I highly recommend sticking to the command line. You will get better at it, and it will help you greatly in the long run.

Formally, Codio is providing a "container" (similar to a virtual machine), which provides the illusion of a full fledged operating system. From your standpoint, think of each Codio box (for every assignment) as a fresh computer, provided just for that assignment. The different Codio boxes are completely independent of each other. Only you and me (the instructor) has access to this machine. In reality, each of these boxes is hosted in the cloud.


