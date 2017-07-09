
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
	reader.generateProgram();

	std::vector<JunoInstruction*> instructions = reader.getInstructions();

	for(int i = 0; i < instructions.size(); ++i) {
		printf("%s ops=%d\n", instructions[i]->getInstCode().c_str(),
			instructions[i]->countOperands());
	}

	printf("Done.\n");
}
