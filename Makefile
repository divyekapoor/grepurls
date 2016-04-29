SOURCES=$(wildcard *.cc)
HEADERS=$(wildcard *.h)
OBJECTS=$(SOURCES:.cc=.o)
BINS=$(SOURCES:.cc=)
DEPS=$(OBJECTS:.o=.d)
INSTALLPATH=/usr/local/bin

CXXFLAGS+=-std=c++11 -Wall -g -O3 -MMD -MP -MF $(DEPS) -I ./PEGTL/ -I ./gflags/build/include
LDFLAGS+=-L ./gflags/build/lib -static
LDLIBS+=-lgflags -lpthread

# Pattern rule for compilation of CC files.
%: %.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(filter %.cc,$^) $(LDLIBS) -o $@

all: $(BINS) test

test: $(BINS) run_tests.sh
	./run_tests.sh

install: $(BINS)
	@echo Installing to $(INSTALLPATH)
	install $(BINS) $(INSTALLPATH)
	install $(BINS) $(INSTALLPATH)/ugrep

uninstall:
	rm $(INSTALLPATH)/$(BINS)
	rm $(INSTALLPATH)/ugrep

.PHONY: all clean test install uninstall

clean:
	$(RM) $(OBJECTS) $(BINS) $(DEPS)

-include $(DEPS)
