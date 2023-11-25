#!/bin/bash
# Runs program and cats output file
make
cat in.txt
echo --------- ./treewrapper  ----------
./treewrapper in.txt out.txt
cat out.txt
make clean
