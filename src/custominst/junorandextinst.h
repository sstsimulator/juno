
#ifndef _H_SST_JUNO_RAND_UNIT
#define _H_SST_JUNO_RAND_UNIT

#include <sst/core/link.h>
#include <sst/core/elementinfo.h>
#include <sst/core/subcomponent.h>
#include <sst/core/rng/mersenne.h>
#include "custominst/junocustinst.h"

#include "junoregfile.h"
#include "junoldstunit.h"

using namespace SST::RNG;

namespace SST {
namespace Juno {

#define JUNO_RAND	200
#define JUNO_RSEED	201

class JunoExternalRandInstructionHandler : public JunoCustomInstructionHandler {

public:
	JunoExternalRandInstructionHandler( Component* owner, Params& params );
	~JunoExternalRandInstructionHandler();

	// Return true if the op-code is either RAND or RSEED instructions
	// that's all we can process in this unit
	bool canProcessInst( const uint8_t opCode );
	void executeRand( SST::Output* output, const JunoCPUInstruction* inst,
                JunoRegisterFile* regFile );
	void executeRandSeed( SST::Output* output, const JunoCPUInstruction* inst,
                JunoRegisterFile* regFile );
	int execute( SST::Output* output, JunoCPUInstruction* inst,
                JunoRegisterFile* regFile, JunoLoadStoreUnit* loadStoreUnit,
		uint64_t* pc );
	bool isBusy();
	void handleGenerateResp(SST::Event* ev);

	SST_ELI_REGISTER_SUBCOMPONENT(
		JunoExternalRandInstructionHandler,
		"juno",
		"JunoExternalRandomHandler",
		SST_ELI_ELEMENT_VERSION(1, 0, 0),
		"Random number generation instruction handler that connects to an external random number generator for Juno",
		"SST::Juno::CustomInstructionHandler"
		)

	SST_ELI_DOCUMENT_PORTS(
		{ "genlink", "Link to the Random number accelerator", { "juno.JunoGenerateRandEvent", "" } }
		)

private:
	int nextEvID;
	uint8_t targetReg;
	JunoRegisterFile* registers;
	SST::Link* randAccLink;
	SST::Output* cpuOut;

};

}
}

#endif
