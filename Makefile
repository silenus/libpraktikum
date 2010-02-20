CFLAGS=-Wall -Werror
LDFLAGS=
INCROOT=$(shell root-config --incdir)
ROOTOPT=-pthread

GPP=g++
CC=gcc
DOXY=doxygen

OBJECTS=utils.o data.o
SUBDIRS=

.PHONY: all clean doc $(SUBDIRS)

all: $(OBJECTS) $(SUBDIRS)
	@echo "done";

%.o:    %.c | %.h
	$(CC) -c $< $(CFLAGS)

%.o:    %.cpp | %.h
	$(GPP) -c $< $(CFLAGS) -I$(INCROOT) $(ROOTOPT)

$(SUBDIRS):
	@cd $@; $(MAKE);

doc:
	$(DOXY) Doxygen;

clean:
	@echo "CLEAN";
	rm -f *.o
	@for dir in $(SUBDIRS); do ( cd $$dir; $(MAKE) clean; ); done
