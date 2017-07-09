
ASM_SOURCES := $(wildcard *.cc)
ASM_HEADERS := $(wildcard *.h)
JUNO_OP_CODES := ../src/junoopcodes.h

JUNO_OBJS := $(patsubst %.cc,%.o,$(ASM_SOURCES))

CXX=g++
CPPFLAGS=-I. -I../src -O3 -Wall
CXXFLAGS=-std=c++11 -g

sst-juno-asm: $(JUNO_OBJS) $(ASM_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $(JUNO_OBJS)

clean:
	rm sst-juno-asm *.o
