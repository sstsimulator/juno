CXX=$(shell sst-config --CXX)
CXXFLAGS=$(shell sst-config --ELEMENT_CXXFLAGS)
LDFLAGS=$(shell sst-config --ELEMENT_LDFLAGS)
CPPFLAGS=-I./ -I./assembly

all: libjuno.so install

libjuno.so: junocpu.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $<

install:
	sst-register juno LIBDIR=$(CURDIR)

clean:
	rm -f *.o libjuno.so
