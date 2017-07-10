
#ifndef _H_SST_JUNO_JUMP_CONTROL
#define _H_SST_JUNO_JUMP_CONTROL

#include <cinttypes>

#include "junocpuinst.h"
#include "junoregfile.h"

namespace SST {
namespace Juno {

void executeJumpZero( SST::Output& output, JunoCPUInstruction* inst, RegisterFile* regFile, uint64_t* pc ) {
        const uint8_t chkReg    = inst->getReadReg1();
        const int64_t regVal   = regFile->readReg(chkReg);

	const int16_t pcDiff   = inst->get16bJumpOffset() * 4;
	const uint64_t pcOut   = static_cast<uint64_t>( (0 == regVal) ? static_cast<int64_t>(*pc) + static_cast<int64_t>(pcDiff) : (*pc) + 4);

	output.verbose(CALL_INFO, 4, 0, "JZERO[r%3" PRIu8 ", offset=%" PRId16 "] (%" PRId64 ", pcIn=%" PRId64 ", pcOut=%" PRId64 ")\n",
		chkReg, pcDiff, regVal, (*pc), pcOut);

	*pc = pcOut;
};

}
}

#endif
