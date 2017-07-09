
#ifndef _H_SST_JUNO_ASM_PROGRAM
#define _H_SST_JUNO_ASM_PROGRAM

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
		int64Literals = new std::vector<int64_t>();
	}

	~AssemblyProgram() {
		delete op;
		delete int64Literals;
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

protected:
	std::map<std::string, uint64_t> labelMap;
	std::vector<int64_t>* int64Literals;
	std::vector<AssemblyOperation*>* op;

};

}
}
}

#endif
