SOURCES=$(wildcard *.cc)
OBJECTS=$(SOURCES:.cc=.o)
BINS=$(SOURCES:.cc=)
INSTALLPATH=/usr/local/bin

CXXFLAGS+=-std=c++11 -Wall -g -O3 -IPEGTL/ -Igflags/build/include
LDFLAGS+=-Lgflags/build/lib
LDLIBS+=-lgflags -lpthread

all: $(BINS) test

test: $(BINS) run_tests.sh
	./run_tests.sh

install: $(BINS)
	@echo Installing to $(INSTALLPATH)
	cp $(BINS) $(INSTALLPATH)
	ln -s $(INSTALLPATH)/$(BINS) $(INSTALLPATH)/ugrep

uninstall:
	rm $(INSTALLPATH)/$(BINS)
	rm $(INSTALLPATH)/ugrep

.PHONY: clean test

clean:
	$(RM) $(OBJECTS) $(BINS)

