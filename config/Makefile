CC=g++
CXX=g++
CXXFLAGS=-Wall -std=c++20
VPATH=.:../src
CPPFLAGS=-I. -I../src -I/usr/include

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

helpers/linetools.o:	helpers/linetools.cpp helpers/linetools.h
gfffile.o:	gfffile.cpp gfffile.h gffentry.h locus/featurepart.h helpers/linetools.h
ivalnode.o:	ivalnode.cpp ivalnode.h gffentry.h
overlap.o: overlap.cpp overlap.h
gffentry.o: gffentry.cpp gffentry.h helpers/linetools.h
locus.o: locus.cpp locus.h
entrydb.o:	entrydb.cpp entrydb.h
isoformscanner.o: isoformscanner.cpp isoformscanner.h gfffile.h overlap.h ivalnode.h locus.h entrydb.h
isoformscanner: isoformscanner.o	gfffile.o overlap.o ivalnode.o gffentry.o helpers/linetools.o locus.o entrydb.o
