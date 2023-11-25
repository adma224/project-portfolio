#-----------------------------------------------------------------------------------------
#-----------------------------------------------------------------------------------------
# Author: 
# 	Adrian Murillo
# CruzID: 
# 	amurill9
# ID#: 
# 	1573260
# Institution:
# 	UC Santa Cruz
# Course:
# 	CMPS 101
# File Name:
# 	Matrix.java
# File Description:
# 	Matrix ADT functions
#-----------------------------------------------------------------------------------------
#  Creates an executable jar file called Sparse.  This makefile will compile 
#  all .java files in your current working directory.  
#-----------------------------------------------------------------------------------------

MAINCLASS  = Sparse
JAVAC      = javac 
JAVASRC    = $(wildcard *.java)
SOURCES    = $(JAVASRC) Makefile README
CLASSES    = $(patsubst %.java, %.class, $(JAVASRC))
JARCLASSES = $(patsubst %.class, %*.class, $(CLASSES))
JARFILE    = $(MAINCLASS) 

all: $(JARFILE)
	
$(JARFILE): $(CLASSES)
	echo Main-class: $(MAINCLASS) > Manifest
	jar cvfm $(JARFILE) Manifest $(JARCLASSES)
	chmod +x $(JARFILE)
	rm Manifest

%.class: %.java
	$(JAVAC) $<

clean:
	rm -f *.class $(JARFILE)