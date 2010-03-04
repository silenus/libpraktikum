CFLAGS=-Wall -Werror -g
LDFLAGS=
LIBROOT=$(shell root-config --libdir)
INCROOT=$(shell root-config --incdir)
LIBS=-lCint -lCore -lHist -lMathCore -lPhysics -lGpad -lGraf -lMatrix -lRint -lRIO -lNet -lRint -lGraf3d -lTree -lThread -lGui -ldl -lz -lSpectrum
ROOTOPT=-pthread

GPP=g++
CC=gcc

OBJECTS=linear.o
PROGRAMMS=linear

.PHONY: all clean doc $(SUBDIRS)

all: $(PROGRAMMS) $(OBJECTS) $(SUBDIRS)
	@echo "done";

plot: plot.o
	$(GPP) $^ ../data.o ../utils.o -o plot -L$(LIBROOT) $(LIBS)

linear: linear.o
	$(GPP) $^ ../linearregression.o ../data.o ../utils.o -o linear -L$(LIBROOT) $(LIBS)

%.o:    %.c
	$(CC) -c $< $(CFLAGS)

%.o:    %.cpp
	$(GPP) -c $< $(CFLAGS) -I$(INCROOT) $(ROOTOPT)

clean:
	@echo "CLEAN";
	rm -f *.o $(PROGRAMMS)
	@for dir in $(SUBDIRS); do ( cd $$dir; $(MAKE) clean; ); done
