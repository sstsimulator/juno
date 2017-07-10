
#ifndef _H_SST_JUNO_LOAD_STORE
#define _H_SST_JUNO_LOAD_STORE

#include <cinttypes>

#include "junocpuinst.h"
#include "junoregfile.h"

namespace SST {
namespace Juno {

void executeLDA( SST::Output& output, JunoCPUInstruction* inst, JunoRegisterFile* regFile, JunoLoadStoreUnit* ldst ) {

     	const uint8_t resultReg = inst->getWriteReg();
     	const uint16_t addrLit = inst->get16bAbsAddr();

	output.verbose(CALL_INFO, 4, 0, "LDA[%" PRIu16 ", res=r%" PRIu8 "]\n", addrLit, resultReg);

	ldst->createLoadRequest( static_cast<uint64_t>(addrLit), resultReg );

};

}
}

#endif
