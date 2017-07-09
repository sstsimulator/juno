
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "asmop.h"
#include "asmreader.h"
#include "asmoptions.h"

using namespace SST::Juno::Assembler;

AssemblyReader::AssemblyReader( AssemblerOptions* ops ) :
	options( ops ) {}

AssemblyReader::~AssemblyReader() {}

int64_t AssemblyReader::convertLiteralFromString(const char* litStr) {
	if( strlen(litStr) < 2 ) {
		fprintf(stderr, "Error: literal string \"%s\" cannot be converted to literal value.\n", litStr);
		exit(-1);
	}

	if( '$' != litStr[0] ) {
		fprintf(stderr, "Error: literal string \"%s\" does not start with a $.\n", litStr);
		exit(-1);
	}

	return std::atoll( &litStr[1] );
}

int AssemblyReader::getLiteralIndex(const int64_t checkLit) {
/*
	const int literalCount = literals.size();

	int index = -1;

	for(int i = 0; i < literalCount; ++i) {
		if( checkLit == literals[i] ) {
			index = i;
			break;
		}
	}

	if( -1 == index ) {
		literals.push_back( checkLit );
		index = literalCount;
	}
*/
	const int index = 0;
	return index;
}

bool AssemblyReader::readLine(char* buffer, const int buffLen) {
	FILE* inFile = options->getInputFile();

	if( feof( inFile ) ) {
		return false;
	} else {
		int nextChar = fgetc( inFile );
		int nextIndex = 0;

		if( ('\n' == nextChar) || ('\r' == nextChar) || ('\f' == nextChar) ) {

		} else {
			while( (EOF != nextChar) ) {
				buffer[nextIndex] = (char) nextChar;
				nextIndex++;

				nextChar = fgetc( inFile );

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

AssemblyProgram* AssemblyReader::assemble() {
	AssemblyProgram* program;

	#define JUNO_MAX_LINE_LEN 2048

	char* buffer = (char*) malloc( sizeof(char) * JUNO_MAX_LINE_LEN );
	int currentLine = 1;

	while( readLine(buffer, JUNO_MAX_LINE_LEN) ) {
		printf("Line[%s]\n", buffer);

		if( strlen(buffer) > 1 ) {
			// Allow for comments, we ignore them
			if( '#' != buffer[0] ) {
				char* inst = strtok( buffer, " " );

				AssemblyOperation* newInst = new AssemblyOperation(inst);
				program->addOperation(newInst);

				char* op = strtok(NULL, " ");
				while( NULL != op ) {
					if( op[0] == 'r' || op[0] == 'R' ) {
						printf("Making a register from: %s\n", op);
						newInst->addOperand( new AssemblyRegisterOperand( op ) );
					} else if( op[ strlen(op) - 1 ] == ':' ) {
						printf("Found a label: %s\n", op);
//						labelMap.insert( std::pair<std::string, int64_t>(
//							, instructions.size() );
					} else if( op[0] == '$' ) {
						// Literal?
						printf("Making a literal from: %s\n", op);
						const int64_t literal = convertLiteralFromString(op);

						int literalIndex = getLiteralIndex( literal );

						printf("Found at index: %d\n", literalIndex);

						AssemblyLiteralOperand* newLit = new AssemblyLiteralOperand( op );
						newInst->addOperand( newLit );
					} else {
						printf("Making a memory from: %s\n", op);
						newInst->addOperand( new AssemblyMemoryOperand( op ) );
						// Literal?
					}

					op = strtok( NULL, " " );
				}
			}
		}

		currentLine++;
	}

	free(buffer);
	
	return program;
}

