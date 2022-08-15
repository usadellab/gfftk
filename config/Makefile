CC=g++
CXX=g++
CXXFLAGS=-Wall -std=c++20
VPATH=.:../src
CPPFLAGS=-I. -I../src -I/usr/include

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

utils.o:	utils.cpp utils.h
gfffile.o:	gfffile.cpp gfffile.h gffentry.h utils.h
ivalnode.o:	ivalnode.cpp ivalnode.h gffentry.h
overlap.o: overlap.cpp overlap.h
gffentry.o: gffentry.cpp gffentry.h utils.h
locus.o: locus.cpp locus.h
entrydb.o:	entrydb.cpp entrydb.h
isoformscanner.o: isoformscanner.cpp isoformscanner.h gfffile.h overlap.h ivalnode.h locus.h entrydb.h
isoformscanner: isoformscanner.o	gfffile.o overlap.o ivalnode.o gffentry.o utils.o locus.o entrydb.o
