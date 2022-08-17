CXX=g++
CXXFLAGS=-Wall -std=c++20
VPATH=.:../src
INCLUDES=-I. -I../src -I/usr/include

objects = linetools.o locus.o gfffile.o gffentry.o isoformscanner.o

.PHONY : all clean

# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

isoformscanner: isoformscanner.o gfffile.o gffentry.o linetools.o locus.o
	$(CXX) $(CXXFLAGS) $(INCLUDES)  -o $@

isoformscanner.o: isoformscanner.cpp isoformscanner.h gfffile.h gffentry.h locus/locus.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

gfffile.o: gfffile.cpp gfffile.h locus/featurepart.h helpers/linetools.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

linetools.o:	helpers/linetools.cpp helpers/linetools.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

gffentry.o: gffentry.cpp gffentry.h helpers/linetools.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

locus.o: locus/locus.cpp locus/locus.h gffentry.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


clean:
	-rm isoformscanner $(objects)

all:	isoformscanner


# CC=g++
# CXX=g++
# CXXFLAGS=-Wall -std=c++20
# VPATH=.:../src
# CPPFLAGS=-I. -I../src -I/usr/include

# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# linetools.o:	helpers/linetools.cpp helpers/linetools.h
# gfffile.o:	gfffile.cpp gfffile.h gffentry.h locus/featurepart.h helpers/linetools.h
# ivalnode.o:	ivalnode.cpp ivalnode.h gffentry.h
# overlap.o: overlap.cpp overlap.h
# gffentry.o: gffentry.cpp gffentry.h helpers/linetools.h
# locus.o: locus.cpp locus.h
# entrydb.o:	entrydb.cpp entrydb.h
# isoformscanner.o: isoformscanner.cpp isoformscanner.h gfffile.h overlap.h ivalnode.h locus.h entrydb.h
# isoformscanner: isoformscanner.o	gfffile.o overlap.o ivalnode.o gffentry.o helpers/linetools.o locus.o entrydb.o
