CFLAGS=-Wall -Werror
LDFLAGS=

GPP=g++
CC=gcc

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
	@echo "DOC\t<<FIXME:WRITE DOXYGEN HERE>>";

clean:
	@echo "CLEAN";
	rm -f *.o
	@for dir in $(SUBDIRS); do ( cd $$dir; $(MAKE) clean; ); done
