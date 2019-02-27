###############################################################
# Program:
#     Lab 08, OpenMP
#     Brother Ball, CS345
# Author:
#     Ted McDaniel
# Summary:
#     Matrix Multiplication with OpenMP
# Time:
#     <how long did it take to complete this program?>
###############################################################

##############################################################
# The main rule
##############################################################
a.out: lab08.cpp
	g++ -o a.out -fopenmp lab08.cpp 
	tar -czf lab08.tar makefile lab08.cpp

sampleVariableThreads.out: sampleVariableThreads.cpp
	g++ -o sampleVariableThreads.out -fopenmp sampleVariableThreads.cpp

all: a.out sampleVariableThreads.out
