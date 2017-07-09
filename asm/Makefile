
CXX=g++
CPPFLAGS=-I.
CXXFLAGS=-std=c++11 -g

junoasm: asmmain.o asmreader.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ asmmain.o asmreader.o

clean:
	rm junoasm *.o
