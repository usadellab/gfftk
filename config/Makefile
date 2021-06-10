CC=g++
CXX=g++
CXXFLAGS=-Wall -std=c++20
VPATH=.:../src
CPPFLAGS=-I. -I../src -I/usr/include

reader.o:	reader.cpp reader.h gffentry.h
ivalnode.o:	ivalnode.cpp ivalnode.h gffentry.h
overlap.o: overlap.cpp overlap.h
isoformscanner.o:	isoformscanner.cpp isoformscanner.h reader.h overlap.h ivalnode.h
isoformscanner:	isoformscanner.o	reader.o overlap.o ivalnode.o
