SOURCES=$(wildcard *.cc)
OBJECTS=$(SOURCES:.cc=.o)
BINS=$(SOURCES:.cc=)

CXXFLAGS+=-std=c++11 -Wall -g -O3 -IPEGTL/

all: $(BINS) test

test: $(BINS) run_tests.sh
	./run_tests.sh

install: $(BINS)
	cp $(BINS) /usr/local/bin/

.PHONY: clean test

clean:
	$(RM) $(OBJECTS) $(BINS)

