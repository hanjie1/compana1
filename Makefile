# Makefile for analysis of JLab FADC data
#

   ifdef OLDROOT
     ROOTLIBS      = -L$(ROOTSYS)/lib -lNew -lBase -lCint -lClib -lCont -lFunc \
                    -lGraf -lGraf3d -lHist -lHtml -lMatrix -lMeta -lMinuit -lNet \
                    -lPhysics -lPostscript -lProof -lRint -lTree -lUnix -lZip
     ROOTGLIBS     = -lGpad -lGui -lGX11 -lX3d
   else
     ROOTLIBS      = $(shell root-config --libs)
     ROOTGLIBS     = $(shell root-config --glibs)
   endif

   CXX           = g++
   CXXFLAGS      = -Wall  -fno-exceptions -fPIC  \
                   -DLINUXVERS -I$(ROOTSYS)/include -O

# Linux with egcs
   INCLUDES      = -I$(ROOTSYS)/include -I$(EVIO_INCDIR) -I$(EVIO)/common/include
   CXX           = g++
   CXXFLAGS      = -O -Wall  -fno-exceptions  -fPIC $(INCLUDES)
   CXXFLAGS     += -Wno-deprecated -std=c++11
   LD            = g++
   LDFLAGS       = 
   SOFLAGS       = -shared

   ifdef OLDROOT
      LIBS          = $(ROOTLIBS) -lm -ldl -rdynamic
      GLIBS         = $(ROOTLIBS) $(ROOTGLIBS) -L/usr/X11R6/lib \
                      -lXpm -lX11 -lm -ldl -rdynamic
      CXXFLAGS     += -DOLDROOT
   else
      LIBS          = $(ROOTLIBS)
      GLIBS         = $(ROOTGLIBS) -L/usr/X11R6/lib -lXpm -lX11
   endif

   EVIO_LIB= $(EVIO)/lib/libevio.so
   ALL_LIBS = $(EVIO_LIB) $(GLIBS) $(ROOTLIBS) 

  ifdef PROFILE
     CXXFLAGS += -pg
  endif


#SRC = compana.C
#HEAD = $(SRC:.C=.h)
#DEPS = $(SRC:.C=.d)
#DECODE_OBJS = $(SRC:.C=.o)

ifdef STANDALONE 
  CXXFLAGS += -DSTANDALONE
endif

all: compana

compana: compana.o $(DECODE_OBJS) $(HEAD) $(EVIO)
	$(CXX) -g $(CXXFLAGS) -o $@ compana.C $(DECODE_OBJS) $(ALL_LIBS) 

clean:  
	rm -f *.o *.a core *~ *.d *.out compana


.SUFFIXES:
.SUFFIXES: .c .cc .cpp .C .o .d

%.o:	%.C
	$(CXX) $(CXXFLAGS) -c $<

%.d:	%.C
	@echo Creating dependencies for $<
	@$(SHELL) -ec '$(CXX) -MM $(CXXFLAGS) -c $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

-include $(DEPS)








