CXX=g++
# CXXFLAGS=-Wall -std=c++20
CXXFLAGS=-Wall -std=c++2a -Wno-unused-function
VPATH=.:../src
INCLUDES=-I. -I../src -I/usr/include

objects = gfftk.o stringtools.o locus.o gfffile.o gffrow.o feature.o typefeature.o command.o extractor.o viewer.o fastafile.o

.PHONY : all clean

gfftk.o: gfftk.cpp gfftk.h gfffile.h commands/extractor.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

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

command.o: commands/command.cpp commands/command.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

extractor.o: commands/extractor.cpp commands/extractor.h commands/command.h fasta/fastafile.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

viewer.o: commands/viewer.cpp commands/viewer.h commands/command.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

fastafile.o: fasta/fastafile.cpp fasta/fastafile.h components/feature.h helpers/stringtools.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

gfftk: $(objects)
	$(CXX) $(CXXFLAGS) $(INCLUDES)  -o $@ $+

 clean:
	-rm gfftk $(objects)

all:	gfftk

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
