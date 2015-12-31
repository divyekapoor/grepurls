SOURCES=$(wildcard *.cc)
OBJECTS=$(SOURCES:.cc=.o)
BINS=$(SOURCES:.cc=)

CXXFLAGS+=-std=c++11 -O3 -IPEGTL/

all: $(BINS)

.PHONY: clean

clean:
	$(RM) $(OBJECTS) $(BINS)

