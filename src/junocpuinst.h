
#ifndef _H_SST_JUNO_CPU_INSY_
#define _H_SST_JUNO_CPU_INSY_

#include <cinttypes>
#include "junoopcodes.h"

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
