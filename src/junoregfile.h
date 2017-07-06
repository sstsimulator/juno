
#ifndef _H_SST_JUNO_REG_FILE_
#define _H_SST_JUNO_REG_FILE_

#include <cinttypes>

namespace SST {
namespace Juno {

class RegisterFile {

public:
	RegisterFile( const int regCount ) :
		maxReg(regCount) {

		registers = (int64_t) malloc( sizeof(int64_t) * regCount );
	}

	~RegisterFile() {
		free(registers);
	}

	void clear() {
		for(int i = 0; i < maxReg; ++i) {
			registers[i] = 0;
		}
	}

	int64_t readReg(const int reg) {
		return registers[reg];
	}

	void writeReg(const int reg, int64_t val) {
		registers[reg] = val;
	}

protected:
	const int maxReg;
	int64_t* registers;

};

}
}

#endif
