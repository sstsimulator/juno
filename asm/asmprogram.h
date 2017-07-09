
#ifndef _H_SST_JUNO_ASM_PROGRAM
#define _H_SST_JUNO_ASM_PROGRAM

#include "junoopcodes.h"

#include <vector>
#include <map>
#include <cinttypes>
#include <cstdint>

namespace SST {
namespace Juno {
namespace Assembler {

class AssemblyProgram {

public:
	AssemblyProgram() {
		op = new std::vector<AssemblyOperation*>();
	}

	~AssemblyProgram() {
		delete op;
	}

	std::vector<AssemblyOperation*>* getOperations() {
		return op;
	}

	uint64_t countOperations() {
		return static_cast<uint64_t>(op->size());
	}

	void addOperation( AssemblyOperation* newOp ) {
		op->push_back( newOp );
	}

	void collectLiterals() {
		printf("Collecting literals...\n");

		int64Literals.clear();

		for( auto opItr = op->begin(); opItr != op->end(); opItr++ ) {
			AssemblyOperation* curOp = (*opItr);

			printf("INSTRUCTION{%20s][%d operands]\n", curOp->getInstCode().c_str(),
				curOp->countOperands());

			for( int i = 0 ; i < curOp->countOperands(); ++i ) {
				printf("-> Operand: Type=%d\n", (int) curOp->getOperand(i)->getType());

				if( curOp->getOperand(i)->getType() == LITERAL_OPERAND ) {
					AssemblyLiteralOperand* litOp = dynamic_cast<AssemblyLiteralOperand*>( curOp->getOperand(i) );

					if( NULL == litOp ) {
						fprintf(stderr, "Error: expected a literal operand but cast failed.\n");
						exit(-1);
					}

					const int64_t litVal = litOp->getLiteral();

					printf("Searching for literal value: %" PRId64 "...\n", litVal);

					int index = -1;

					for( int j = 0; j < int64Literals.size(); ++j ) {
						if( litVal == int64Literals[j] ) {
							index = j;
							break;
						}
					}

					printf("Index: %d\n", index);

					// Found a unique literal value
					if( -1 == index ) {
						int64Literals.push_back( litVal );
					}
				}
			}
		}

		printf("Literal collection done (collected: %d literals)\n", int64Literals.size());
	}

	void addLabel( const std::string labelText, const uint64_t loc ) {
		auto checkExists = labelMap.find(labelText);

		if( checkExists == labelMap.end() ) {
			labelMap.insert( std::pair<std::string, uint64_t>(labelText, loc) );
		} else {
			fprintf(stderr, "Error: label \"%s\" is defined twice in this program.\n", labelText.c_str() );
			exit(-1);
		}
	}

	bool verifyLabels() {
		printf("Verifying labels...\n");
		bool foundAll = true;

		for( auto opItr = op->begin(); opItr != op->end(); opItr++ ) {
			AssemblyOperation* curOp = (*opItr);

			for( int j = 0; j < curOp->countOperands(); ++j ) {
				if( LABEL_OPERAND == curOp->getOperand(j)->getType() ) {
					AssemblyLabelOperand* labelOp = dynamic_cast<AssemblyLabelOperand*>( curOp->getOperand(j) );

					if( NULL == labelOp ) {
						fprintf(stderr, "Error: attempted to convert a LABEL_OPERAND to object but failed.\n");
						exit(-1);
					}

					const std::string& labelStr = labelOp->getLabel();

					if( labelMap.find(labelStr) == labelMap.end() ) {
						fprintf(stderr, "Error: did not find label \"%s\" in program.\n", labelStr.c_str());
						foundAll = false;
					}
				}
			}
		}

		printf("Completed checking labels\n");

		if( ! foundAll ) {
			fprintf(stderr, "Error: could not find all labels in the program.\n");
			exit(-1);
		}

		return true;
	}

	int countInt64Literals() {
		return static_cast<int>( int64Literals.size() );
	}

	int64_t getInt64Literal( const int index ) {
		return int64Literals.at( index );
	}

	void writeBinary( FILE* binary ) {
		const int64_t  BINARY_VERSION = 1000000;
		const uint64_t literalSize = static_cast<uint64_t>(int64Literals.size()) * 8;
		const uint64_t programSize = static_cast<uint64_t>(op->size()) * 4;

		// Write out the header file
		fwrite( &BINARY_VERSION, sizeof(BINARY_VERSION), 1, binary );
		fwrite( &literalSize, sizeof(literalSize), 1, binary );
		fwrite( &programSize, sizeof(programSize), 1, binary );

		for( int i = 0; i < int64Literals.size(); ++i ) {
			fwrite( &int64Literals[i], sizeof(int64_t), 1, binary );
		}

		char binaryOp[4];

		for( int i = 0; i < op->size(); ++i ) {
			AssemblyOperation* curOp = op->at(i);

			if( curOp->getInstCode() == "ADD" ) {
				generateBinaryOperand( JUNO_ADD, curOp, binaryOp );
			} else if( curOp->getInstCode() == "SUB" ) {
				generateBinaryOperand( JUNO_SUB, curOp, binaryOp );
			} else if( curOp->getInstCode() == "MUL" ) {
				generateBinaryOperand( JUNO_MUL, curOp, binaryOp );
			} else if( curOp->getInstCode() == "DIV" ) {
				generateBinaryOperand( JUNO_DIV, curOp, binaryOp );
			} else if( curOp->getInstCode() == "AND" ) {
				generateBinaryOperand( JUNO_AND, curOp, binaryOp );
			} else if( curOp->getInstCode() == "OR" ) {
				generateBinaryOperand( JUNO_OR, curOp, binaryOp );
			} else if( curOp->getInstCode() == "XOR" ) {
				generateBinaryOperand( JUNO_XOR, curOp, binaryOp );
			} else {
				fprintf(stderr, "Unknown instruction code: %s at instruction index: %d\n",
					curOp->getInstCode().c_str(), i);
				exit(-1);
			}

			fwrite( &binaryOp[0], sizeof(char), 4, binary );
		}
	}

	void generateBinaryOperand( const char junoCode, AssemblyOperation* curOp, char* binaryOp ) {
				memcpy( (void*) &binaryOp[0], (void*) &junoCode, sizeof(junoCode) );

				if( curOp->countOperands() != 3 ) {
					fprintf(stderr, "Error: instruction (%s) must have three operands, it only has %d\n",
						curOp->getInstCode().c_str(), curOp->countOperands());
					exit(-1);
				}

				populateBinaryOperand( curOp, &binaryOp[1] );
	}

	void populateBinaryOperand( AssemblyOperation* curOp, char* binaryOp ) {
		for( int j = 0; j < 3; ++j ) {
                	AssemblyOperand* curOperand = curOp->getOperand(j);

                                        if( curOperand->getType() != REGISTER_OPERAND ) {
                                                fprintf(stderr, "Error: %s operation only allows register operands, operand %d is not a register.\n",
                                                        curOp->getInstCode().c_str(), j);
                                        }

                                        AssemblyRegisterOperand* curRegOperand = dynamic_cast<AssemblyRegisterOperand*>( curOperand );
					const uint8_t reg = curRegOperand->getRegister();

                                        memcpy( (void*) &binaryOp[j], (void*) &reg, sizeof(char) );
                                }
	}

protected:
	std::map<std::string, uint64_t> labelMap;
	std::vector<int64_t> int64Literals;
	std::vector<AssemblyOperation*>* op;

};

}
}
}

#endif
