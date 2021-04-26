CC=g++
CXX=g++
CXXFLAGS=-Wall -std=c++20
VPATH=.:../src
CPPFLAGS=-I. -I../src -I/usr/include

reader.o:	reader.cpp reader.h
isoformscanner.o:	isoformscanner.cpp isoformscanner.h reader.h
isoformscanner:	isoformscanner.o	reader.o
