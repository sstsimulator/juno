
#ifndef _H_SST_JUNO_RAND_UNIT
#define _H_SST_JUNO_RAND_UNIT

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

class JunoRandInstructionHandler : public JunoCustomInstructionHandler {

public:
	JunoRandInstructionHandler( Component* owner, Params& params );
	~JunoRandInstructionHandler();

	// Return true if the op-code is either RAND or RSEED instructions
	// that's all we can process in this unit
	bool canProcessInst( const uint8_t opCode );
	void executeRand( SST::Output* output, const JunoCPUInstruction* inst,
                JunoRegisterFile* regFile );
	void executeRandSeed( SST::Output* output, const JunoCPUInstruction* inst,
                JunoRegisterFile* regFile );
	int execute( SST::Output* output, JunoCPUInstruction* inst,
                JunoRegisterFile* regFile, JunoLoadStoreUnit* loadStoreUnit,
		uint64_t* pc, SST::Cycle_t* cycles);

	SST_ELI_REGISTER_SUBCOMPONENT(
		JunoRandInstructionHandler,
		"juno",
		"JunoRandomHandler",
		SST_ELI_ELEMENT_VERSION(1, 0, 0),
		"Random number generation instruction handler for Juno",
		"SST::Juno::CustomInstructionHandler"
		)

	SST_ELI_DOCUMENT_PARAMS(
        	{ "seed", "Set the seed value for the random instructions", "101010101" }
    		)

	SST_ELI_DOCUMENT_STATISTICS(
		{ "calls-to-rand", "Number of times the RAND instruction is called", "calls", 1 },
		{ "calls-to-rseed", "Number of times the RSEED instruction is called", "calls", 1 }
		)

private:
	MersenneRNG* rng;
	Statistic<uint64_t>* statRandCalls;
	Statistic<uint64_t>* statRandSeedCalls;

};

}
}

#endif
