
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

	for(int i = instructions.size() - 1; i >= 0; --i) {
		delete instructions[i];
	}
}

int64_t AssemblyReader::convertLiteralFromString(const char* litStr) {
	if( strlen(litStr) < 2 ) {
		fprintf(stderr, "Error: literal string \"%s\" cannot be converted to literal value.\n", litStr);
		exit(-1);
	}

	if( '$' != litStr[0] ) {
		fprintf(stderr, "Error: literal string \"%s\" does not start with a $.\n");
		exit(-1);
	}

	return std::atoll( &litStr[1] );
}

int AssemblyReader::getLiteralIndex(const int64_t checkLit) {
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

	return index;
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
					} else if( op[ strlen(op) - 1 ] == ':' ) {
						printf("Found a label: %s\n", op);
						labelMap.insert( std::pair<std::string, int64_t>(
							, instructions.size() );
					} else if( op[0] == '$' ) {
						// Literal?
						printf("Making a literal from: %s\n", op);
						const int64_t literal = convertLiteralFromString(op);

						int literalIndex = getLiteralIndex( literal );

						printf("Found at index: %d\n", literalIndex);

						JunoLiteralOperand* newLit = new JunoLiteralOperand( op );
						newInst->addOperand( newLit );
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
