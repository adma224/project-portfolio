#!/bin/bash
cd NQueens
make
./nqueens simple-input.txt simple-output.txt
make clean
cd
