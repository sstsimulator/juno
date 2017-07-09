
#ifndef _H_SST_JUNO_ASM_PROGRAM
#define _H_SST_JUNO_ASM_PROGRAM

namespace SST {
namespace Juno {
namespace Assembler {

class AssemblyProgram {

public:
	AssemblyProgram() {

	}

	~AssemblyProgram() {

	}

	std::vector<AssemblyOperation*>& getOperations() {
		return op;
	}

	void addOperation( AssemblyOperation* newOp ) {
		op.push_back( newOp );
	}

	void collectLiterals() {

	}

protected:
	std::vector<int64_t> int64Literals;
	std::vector<AssemblyOperation*> op;

};

}
}
}

#endif
