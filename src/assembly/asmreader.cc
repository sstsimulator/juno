
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "asminst.h"
#include "asmreader.h"

using namespace SST::Juno;

AssemblyReader::AssemblyReader( const char* progPath ) {
	progFile = fopen( progPath, "r" );
}

AssemblyReader::~AssemblyReader() {
	if( NULL != progFile ) {
		fclose( progFile );
	}
}

bool AssemblyReader::readLine(char* buffer, const int buffLen) {
	if( feof( progFile ) ) {
		return false;
	} else {
		int nextChar = fgetc( progFile );
		int nextIndex = 0;

		if( ('\n' == nextChar) || ('\r' == nextChar) || ('\f' == nextChar) ) {

		} else {
			while( (EOF != nextChar) ) {
				buffer[nextIndex] = (char) nextChar;
				nextIndex++;

				nextChar = fgetc( progFile );

				if( ( '\n' == nextChar ) ||
				    ( '\r' == nextChar ) ||
				    ( '\f' == nextChar ) ) {

					break;
				}

				if( (nextIndex + 1) > buffLen ) {
					fprintf(stderr, "Error: assembly warning - exceeded maximum line length\n");
					break;
				}
			}
		}

		buffer[nextIndex] = '\0';

		return (nextIndex > 0);
	}
}

void AssemblyReader::assemble() {
	#define JUNO_MAX_LINE_LEN 2048

	char* buffer = (char*) malloc( sizeof(char) * JUNO_MAX_LINE_LEN );
	int currentLine = 1;

	std::vector<JunoInstruction*> instructions;

	while( readLine(buffer, JUNO_MAX_LINE_LEN) ) {
		printf("Line[%s]\n", buffer);

		if( strlen(buffer) > 1 ) {
			// Allow for comments, we ignore them
			if( '#' != buffer[0] ) {
				char* inst = strtok( buffer, " " );

				JunoInstruction* newInst = new JunoInstruction(inst);
				instructions.push_back(newInst);

				char* op = strtok(NULL, " ");
				while( NULL != op ) {
					if( op[0] == 'r' || op[0] == 'R' ) {
						printf("Making a register from: %s\n", op);
						newInst->addOperand( new JunoRegisterOperand( op ) );
					} else if( op[0] == '$' ) {
						// Literal?
						printf("Making a literal from: %s\n", op);
					} else {
						printf("Making a memory from: %s\n", op);
						newInst->addOperand( new JunoMemoryOperand( op ) );
						// Literal?
					}

					op = strtok( NULL, " " );
				}
			}
		}

		currentLine++;
	}

	free(buffer);
}
