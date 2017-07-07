CXX=$(shell sst-config --CXX)
CXXFLAGS=$(shell sst-config --ELEMENT_CXXFLAGS)
LDFLAGS=$(shell sst-config --ELEMENT_LDFLAGS)
CPPFLAGS=-I./ -I./assembly

JUNO_SOURCES := $(wildcard *.cc)
ASM_SOURCES := $(wildcard assembly/*.cc)
INSTMGR_SOURCES := $(wildcard instmgr/*.cc)

JUNO_HEADERS := $(wildcard *.h)
ASM_HEADERS := $(wildcard assembly/*.h)
INSTMGR_HEADERS := $(wildcard instmgr/*.h)

JUNO_OBJS := $(patsubst %.cc,%.o,$(wildcard *.cc))
ASM_OBJS := $(patsubst %.cc,%.o,$(wildcard assembly/*.cc))
INSTMGR_OBJS := $(patsubst %.cc,%.o,$(wildcard instmgr/*.cc))

all: libjuno.so install

libjuno.so: $(JUNO_OBJS) $(ASM_OBJS) $(INSTMGR_OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ \
		$(JUNO_OBJS) $(ASM_OBJS) $(INSTMGR_OBJS)

install:
	sst-register juno LIBDIR=$(CURDIR)

clean:
	rm -f *.o assembly/*.o instmgr/*.o libjuno.so
