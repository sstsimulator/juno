
#ifndef _H_SST_JUNO_CUSTOM_INST
#define _H_SST_JUNO_CUSTOM_INST

#include <sst/core/subcomponent.h>

#include "junocpuinst.h"
#include "junoregfile.h"
#include "junoldstunit.h"

namespace SST {
namespace Juno {

class JunoCustomInstructionHandler : public SST::SubComponent {

public:
	JunoCustomInstructionHandler( Component* owner, Params& params ) : SubComponent(owner) {}
	~JunoCustomInstructionHandler() {}

	virtual bool canProcessInst( const uint8_t opCode ) = 0;
	virtual int  execute( SST::Output* output, JunoCPUInstruction* inst,
		JunoRegisterFile* regFile, JunoLoadStoreUnit* loadStoreUnit,
		uint64_t* pc, SST::Cycle_t* cyclesTaken ) = 0;

};

}
}

#endif
