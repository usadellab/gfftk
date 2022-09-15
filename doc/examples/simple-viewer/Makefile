CXX=g++
CXXFLAGS=-Wall -std=c++20
VPATH=.:../../../src
INCLUDES=-I. -I../../../src -I/usr/include

objects = simple-viewer.o stringtools.o locus.o gfffile.o gffrow.o feature.o typefeature.o

.PHONY : all clean

stringtools.o:	helpers/stringtools.cpp helpers/stringtools.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

locus.o:	components/locus.cpp components/locus.h components/gffrow.h components/feature.h components/typefeature.h gfftypes.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

gfffile.o:	gfffile.cpp gfffile.h components/gffrow.h components/locus.h helpers/stringtools.h components/typefeature.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

gffrow.o:	components/gffrow.cpp components/gffrow.h helpers/stringtools.h gfftypes.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

feature.o:	components/feature.cpp components/feature.h components/gffrow.h gfftypes.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

typefeature.o:	components/typefeature.cpp components/typefeature.h components/feature.h components/gffrow.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

simple-viewer.o: simple-viewer.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

simple-viewer: $(objects)
	$(CXX) $(CXXFLAGS) $(INCLUDES)  -o $@ $+

 clean:
	-rm simple-viewer $(objects)

all:	simple-viewer
