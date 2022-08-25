CXX=g++
CXXFLAGS=-Wall -std=c++20
VPATH=.:../src
INCLUDES=-I. -I../src -I/usr/include

objects = linetools.o locus.o gfffile.o isoformscanner.o gffrow.o typefeature.o feature.o

.PHONY : all clean

gfffile.o:	gfffile.cpp gfffile.h components/gffrow.h components/locus.h helpers/linetools.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

linetools.o:	helpers/linetools.cpp helpers/linetools.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

gffrow.o:	components/gffrow.cpp components/gffrow.h helpers/linetools.h gfftypes.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

feature.o:	components/feature.cpp components/feature.h components/gffrow.h gfftypes.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

typefeature.o:	components/typefeature.cpp components/typefeature.h components/feature.h components/gffrow.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

locus.o:	components/locus.cpp components/locus.h components/gffrow.h components/feature.h components/typefeature.h gfftypes.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

isoformscanner.o:	isoformscanner.cpp isoformscanner.h gfffile.h components/locus.h
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
