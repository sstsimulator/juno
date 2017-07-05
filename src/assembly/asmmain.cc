
#include <cstdio>
#include <cstdlib>

#include "asmreader.h"

using namespace SST::Juno;

int main(int argc, char* argv[]) {

	if( argc < 2 ) {
		fprintf(stderr, "Error: specify a file to assemble!\n");
		exit(-1);
	}

	printf("Assembling: %s...\n", argv[1]);

	AssemblyReader reader(argv[1]);
	reader.assemble();

	printf("Done.\n");
}
