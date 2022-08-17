CXX=g++
CXXFLAGS=-Wall -std=c++20
VPATH=.:../src
INCLUDES=-I. -I../src -I/usr/include

objects = linetools.o locus.o gfffile.o gffentry.o isoformscanner.o

.PHONY : all clean

gfffile.o:	gfffile.cpp gfffile.h locus/featurepart.h helpers/linetools.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

linetools.o:	helpers/linetools.cpp helpers/linetools.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

gffentry.o:	gffentry.cpp gffentry.h helpers/linetools.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

locus.o:	locus/locus.cpp locus/locus.h gffentry.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

isoformscanner.o:	isoformscanner.cpp isoformscanner.h gfffile.h gffentry.h locus/locus.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

isoformscanner:	$(objects)
	$(CXX) $(CXXFLAGS) $(INCLUDES)  -o $@ $+

clean:
	-rm isoformscanner $(objects)

all:	isoformscanner

### Variables
# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
#
# $+: like ‘$^’, but prerequisites listed more than once are duplicated in the
# order they were listed in the makefile. This is primarily useful for use in
# linking commands where it is meaningful to repeat library file names in a
# particular order.
#
#$<: The name of the first prerequisite. If the target got its recipe from an
# implicit rule, this will be the first prerequisite added by the implicit rule
#(see Implicit Rules).

# $@: The file name of the target of the rule. If the target is an archive
# member, then ‘$@’ is the name of the archive file. In a pattern rule that
# has multiple targets (see Introduction to Pattern Rules), ‘$@’ is the name
# of whichever target caused the rule’s recipe to be run.

## %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
# compile all *.cpp to *.o in SAME directory
