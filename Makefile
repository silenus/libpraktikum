CFLAGS=-Wall -Werror
LDFLAGS=

GPP=g++
CC=gcc
DOXY=doxygen

OBJECTS=utils.o
SUBDIRS=

.PHONY: all clean doc $(SUBDIRS)

all: $(OBJECTS) $(SUBDIRS)
	@echo "done";

%.o:    %.c | %.h
	$(CC) -c $< $(CFLAGS)

%.o:    %.cpp | %.h
	$(GPP) -c $< $(CFLAGS)

$(SUBDIRS):
	@cd $@; $(MAKE);

doc:
	$(DOXY) Doxygen;

clean:
	@echo "CLEAN";
	rm -f *.o
	@for dir in $(SUBDIRS); do ( cd $$dir; $(MAKE) clean; ); done
