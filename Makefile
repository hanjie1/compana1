# Makefile for compana decoder
#

SIMPLE_INC	= /home/compton/hanjie/compana1/simple
SIMPLE_LIB  = /home/compton/hanjie/compana1/simple

EVIO_INC	?= /site/coda/3.10/Linux-x86_64/include
EVIO_LIB	?= /site/coda/3.10/Linux-x86_64/lib

ROOTLIBS	= $(shell root-config --libs)
ROOTGLIBS	= $(shell root-config --glibs)
ROOTINC		= $(shell root-config --incdir)

INCLUDES	= -I$(ROOTINC) -I$(EVIO_INC) -I$(SIMPLE_INC)
CXXFLAGS	= -g -Wall -std=c++11 $(INCLUDES)

LIBS	        = $(ROOTLIBS) -L${SIMPLE_LIB} -lsimple
GLIBS         	= $(ROOTGLIBS) -L/usr/X11R6/lib -lXpm -lX11

ALL_LIBS	= -L${EVIO_LIB} -levio $(LIBS) $(GLIBS) $(ROOTLIBS)

SRC		= compana.C
HEAD		= $(SRC:.C=.h)
DEPS		= $(SRC:.C=.d)
DECODE_OBJS	= $(SRC:.C=.o)

all: compana

compana: $(DECODE_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $< $(ALL_LIBS)

clean:
	rm -f ${DECODE_OBJS} ${DEPS} *~ compana


.SUFFIXES:
.SUFFIXES: .c .cc .cpp .C .o .d

%.o: %.C
	$(CXX) $(CXXFLAGS) -c $<

%.d: %.C
	@echo Creating dependencies for $<
	@$(SHELL) -ec '$(CXX) -MM $(CXXFLAGS) -c $< \
	| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
	[ -s $@ ] || rm -f $@'

-include $(DEPS)
