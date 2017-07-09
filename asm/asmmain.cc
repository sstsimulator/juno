
#include <cstdio>
#include <cstdlib>

#include "asmreader.h"
#include "asmoptions.h"
#include "asmprogram.h"

using namespace SST::Juno::Assembler;

int main(int argc, char* argv[]) {

	if( argc < 2 ) {
		fprintf(stderr, "Error: specify a file to assemble!\n");
		exit(-1);
	}

	AssemblerOptions* options = new AssemblerOptions(argc, argv);
	AssemblyReader* reader = new AssemblyReader(options);

	AssemblyProgram* program = reader->assemble();

	printf("Assembly complete, cleaning up ...\n");

	delete program;
	delete reader;
	delete options;

}
