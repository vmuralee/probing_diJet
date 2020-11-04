# Makefile for the ROOT test programs.
# This Makefile shows nicely how to compile and link applications
# using the ROOT libraries on all supported platforms.
#
# Copyright (c) 2000 Rene Brun and Fons Rademakers
#
# Author: Fons Rademakers, 29/2/2000

include Makefile.arch

#------------------------------------------------------------------------------

PROFILECOMBINATIONO    = ProfileCombination.$(ObjSuf) RooPoisson.$(ObjSuf) cintfile.o
PROFILECOMBINATIONS    = ProfileCombination.$(SrcSuf) RooPoisson.$(SrcSuf)
PROFILECOMBINATION     = ProfileCombination$(ExeSuf)

OBJS          = $(PROFILECOMBINATIONO) 

PROGRAMS      = $(PROFILECOMBINATION) 

#------------------------------------------------------------------------------
CINTFILE=cintfile.cpp
CINTOBJ=cintfile.o
INCLUDES=-I. -I$(ROOTSYS)/include
HHLIST=RooPoisson.h
PACKAGE=Combination

#------------------------------------------------------------------------------

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)

all:            $(PROGRAMS)


#$(PROFILECOMBINATIONO) : $(wildcard Test*.cxx )
$(PROFILECOMBINATIONO) : $(wildcard *.cxx ) $(wildcard *.cpp )

$(PROFILECOMBINATION):   $(OBJS)
		   $(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
		   $(MT_EXE)
		   @echo "$@ done"

$(CINTOBJ): RooPoisson.h
	$(ROOTSYS)/bin/rootcint -f $(CINTFILE) -c $(INCLUDES) $(HHLIST) $(PACKAGE)_LinkDef.hh
	ls -lh $(CINTFILE)
	$(CXX) -g $(CXXFLAGS) -c $(CINTFILE) -o $(CINTOBJ) $(INCLUDES)
	ls -lh $(CINTFILE)
	ls -lh $(CINTOBJ)



clean:
		@rm -f $(OBJS) core cintfile.* *.o $(PROGRAMS)

distclean:      clean

.SUFFIXES: .$(SrcSuf)

###

.$(SrcSuf).$(ObjSuf):
	$(CXX) -g $(CXXFLAGS) -c $<
