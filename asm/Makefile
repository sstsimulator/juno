
ASM_SOURCES := $(wildcard assembly/*.cc)
ASM_HEADERS := $(wildcard assembly/*.h)
JUNO_OP_CODES := ../src/junoopcodes.h

JUNO_OBJS := $(patsubst %.cc,%.o,$(wildcard *.cc))

CXX=g++
CPPFLAGS=-I. -O3
CXXFLAGS=-std=c++11 -g

sst-juno-asm: $(JUNO_OBJS) $(ASM_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $(JUNO_OBJS)

clean:
	rm sst-juno-asm *.o
