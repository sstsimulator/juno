
#ifndef _H_SST_JUNO_CPU_INSY_
#define _H_SST_JUNO_CPU_INSY_

#include <cinttypes>

#define JUNO_NOOP   0
#define JUNO_LOAD   1
#define JUNO_STORE  2
#define JUNO_ADD    16
#define JUNO_SUB    17
#define JUNO_MUL    18
#define JUNO_DIV    19
#define JUNO_AND    32
#define JUNO_OR     33
#define JUNO_XOR    34
#define JUNO_NOT    35

#define JUNO_EXIT   255

namespace SST {
namespace Juno {

class JunoCPUInstruction {

public:
	JunoCPUInstruction( const int32_t opCode ) : op(opCode) {}
	~JunoCPUInstruction() {}

	uint8_t getInstCode() const {
		return op & 0xFF;
	}

	uint8_t getReadReg1() const {
		return op & 0xFF00;
	}

	uint8_t getReadReg2() const {
		return op & 0xFF0000;
	}

	uint8_t getWriteReg() const {
		return op & 0xFF000000;
	}

protected:
	int32_t op;

};

}
}

#endif
