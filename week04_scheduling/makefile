###############################################################
# Program:
#     Lab 04, Scheduling Lab
#     Brother Ball, CS345
# Author:
#     Ted McDaniel
# Summary:
#     Implementations of RR, SJF, SRT scheduling algorithms
# Time:
#     1900 -
###############################################################

##############################################################
# The main rule
##############################################################
a.out: lab04.o schedule.o
	g++ -o a.out lab04.o schedule.o
	tar -j -cf lab04.tar makefile *.cpp *.h

##############################################################
# The individual components
##############################################################
lab04.o: lab04.cpp schedule.h 
	g++ -c lab04.cpp

schedule.o: schedule*.h schedule.cpp process.h
	g++ -c schedule.cpp

