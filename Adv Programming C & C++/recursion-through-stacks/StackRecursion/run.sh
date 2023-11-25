#!/bin/bash
make
./stackrecursion in.txt out.txt
make clean
cat out.txt
rm out.txt


