
#include <sst/core/sst_config.h>
#include <sst/core/rng/mersenne.h>

#include "custominst/junorandinst.h"
#include "custominst/junocustinst.h"

using namespace SST::RNG;
using namespace SST::Juno;

JunoRandInstructionHandler::JunoRandInstructionHandler( Component* owner, Params& params ) :
		JunoCustomInstructionHandler( owner, params ) {

	const uint64_t rngSeed = params.find<uint64_t>("seed", 101010101);
	rng = new MersenneRNG( rngSeed );

	statRandCalls = registerStatistic<uint64_t>("calls-to-rand");
	statRandSeedCalls = registerStatistic<uint64_t>("calls-to-rseed");

	cyclesLeft = 0;
}

JunoRandInstructionHandler::~JunoRandInstructionHandler() {
	delete rng;
}

bool JunoRandInstructionHandler::isBusy() {
	if( cyclesLeft > 0 ) {
		cyclesLeft--;
	}

	return (cyclesLeft > 0);
}

// Return true if the op-code is either RAND or RSEED instructions
// that's all we can process in this unit
bool JunoRandInstructionHandler::canProcessInst( const uint8_t opCode ) {
	return (opCode == JUNO_RAND) || (opCode == JUNO_RSEED);
}

void JunoRandInstructionHandler::executeRand( SST::Output* output, const JunoCPUInstruction* inst,
	JunoRegisterFile* regFile ) {

	const uint8_t resultReg = inst->getWriteReg();

	output->verbose(CALL_INFO, 2, 0, "Executing custom RAND instruction: RAND[r%" PRIu8 "]\n", resultReg);

	const int64_t randVal = rng->generateNextInt64();
	regFile->writeReg( resultReg, randVal );
}

void JunoRandInstructionHandler::executeRandSeed( SST::Output* output, const JunoCPUInstruction* inst,
        JunoRegisterFile* regFile ) {

	const uint8_t seedReg = inst->getReadReg1();

	output->verbose(CALL_INFO, 2, 0, "Executing custom RSEED instruction: RSEED[r%" PRIu8 "]\n", seedReg);

	rng->seed( static_cast<uint64_t>( regFile->readReg( seedReg ) ) );
}

int JunoRandInstructionHandler::execute( SST::Output* output, JunoCPUInstruction* inst,
        JunoRegisterFile* regFile, JunoLoadStoreUnit* loadStoreUnit, uint64_t* pc ) {

	switch( inst->getInstCode() ) {
	case JUNO_RAND:
		executeRand( output, inst, regFile );
		// Tell the owning CPU how many cycles this takes
		cyclesLeft = 20;
		statRandCalls->addData(1);
		break;

	case JUNO_RSEED:
		executeRandSeed( output, inst, regFile );
		// Tell the CPU how many cycles this instruction takes
		cyclesLeft = 10;
		statRandSeedCalls->addData(1);
		break;

	default:
		output->fatal(CALL_INFO, -1, "Error: unknown instruction prefix: %" PRIu8 "\n",
			inst->getInstCode());
		return 1;
	}

	(*pc) = (*pc) + 4;
	return 0;
}
