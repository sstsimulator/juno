CXX=$(shell sst-config --CXX)
CXXFLAGS=$(shell sst-config --ELEMENT_CXXFLAGS)
LDFLAGS=$(shell sst-config --ELEMENT_LDFLAGS)
CPPFLAGS=-I./ -I./assembly
OPTIMIZE_FLAGS=-O3

JUNO_SOURCES := $(wildcard *.cc)
INSTMGR_SOURCES := $(wildcard instmgr/*.cc)

JUNO_HEADERS := $(wildcard *.h)
INSTMGR_HEADERS := $(wildcard instmgr/*.h)

JUNO_OBJS := $(patsubst %.cc,%.o,$(wildcard *.cc))
INSTMGR_OBJS := $(patsubst %.cc,%.o,$(wildcard instmgr/*.cc))

all: libjuno.so install

libjuno.so: $(JUNO_OBJS) $(ASM_OBJS) $(INSTMGR_OBJS)
	$(CXX) $(OPTIMIZE_FLAGS) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ \
		$(JUNO_OBJS) $(ASM_OBJS) $(INSTMGR_OBJS)

%.o:%.cc $(JUNO_HEADERS) $(INSTMGR_HEADERS)
	$(CXX) $(OPTIMIZE_FLAGS) $(CXXFLAGS) $(CPPFLAGS) -c $<

install:
	sst-register juno LIBDIR=$(CURDIR)

clean:
	rm -f *.o assembly/*.o instmgr/*.o libjuno.so
