
#ifndef _H_SST_JUNO_REG_FILE_
#define _H_SST_JUNO_REG_FILE_

#include <cinttypes>

namespace SST {
namespace Juno {

class JunoRegisterFile {

public:
	JunoRegisterFile( SST::Output* out, const int regCount, uint64_t* pcIn, const uint64_t dynDataStart ) :
		pc(pcIn), output(out), maxReg(regCount), dynDataLoc(dynDataStart) {

		output->verbose(CALL_INFO, 2, 0, "Creating %d registers...\n", regCount);

		registers = (int64_t*) malloc( sizeof(int64_t) * regCount );
		clear();
	}

	~JunoRegisterFile() {
		for( int i = 0; i < maxReg; ++i ) {
			output->verbose(CALL_INFO, 2, 0, "Register [r%06d]: [%" PRId64 "]\n",
				i, registers[i]);
		}

		free(registers);
	}

	void clear() {
		for(int i = 0; i < maxReg; ++i) {
			registers[i] = 0;
		}
	}

	int64_t readReg(const uint8_t reg) const {
		if( 0 == reg ) {
			return static_cast<int64_t>( *pc );
		} else if( 1 == reg ) {
			return static_cast<int64_t>(dynDataLoc);
		} else {
			return registers[reg];
		}
	}

	void writeReg(const uint8_t reg, int64_t val) {
		if( reg >= maxReg ) {
			output->fatal(CALL_INFO, -1, "Accessed register %" PRIu8 ", but max is: %d.\n", reg, maxReg);
		} else if( reg <= 1 ) {
			output->fatal(CALL_INFO, -1, "Attempting write to register %" PRIu8 " but r0 and r1 are hardware reserved.\n",
				reg);
		} else {
			output->verbose(CALL_INFO, 16, 0, "Writing %" PRId64 " into register %" PRIu8 "...\n",
				val, reg);
			registers[reg] = val;
		}
	}

protected:
	uint64_t* pc;
	uint64_t dynDataLoc;
	SST::Output* output;
	const int maxReg;
	int64_t* registers;

};

}
}

#endif
