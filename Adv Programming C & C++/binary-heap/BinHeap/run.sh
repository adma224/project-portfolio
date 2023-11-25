#!/bin/bash
make
echo ---------- in.txt ----------
cat in.txt
echo ---------- ./heapwrapper ----------
./heapwrapper in.txt out.txt
echo ---------- out.txt ----------
cat out.txt
echo ---------- ----------
make clean
