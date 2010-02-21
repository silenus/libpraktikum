CFLAGS=-Wall -Werror
LDFLAGS=
LIBROOT=$(shell root-config --libdir)
INCROOT=$(shell root-config --incdir)
LIBS=-lCint -lCore -lHist -lMathCore -lPhysics -lGpad -lGraf -lMatrix -lRint -lRIO -lNet -lRint -lGraf3d -lTree -lThread -lGui -ldl -lz -lSpectrum
ROOTOPT=-pthread

GPP=g++
CC=gcc

OBJECTS=plot.o
PROGRAMMS=plot

.PHONY: all clean doc $(SUBDIRS)

all: $(PROGRAMMS) $(OBJECTS) $(SUBDIRS)
	@echo "done";

plot: plot.o
	$(GPP) $^ ../data.o -o plot -L$(LIBROOT) $(LIBS)

%.o:    %.c
	$(CC) -c $< $(CFLAGS)

%.o:    %.cpp
	$(GPP) -c $< $(CFLAGS) -I$(INCROOT) $(ROOTOPT)

clean:
	@echo "CLEAN";
	rm -f *.o $(PROGRAMMS)
	@for dir in $(SUBDIRS); do ( cd $$dir; $(MAKE) clean; ); done
