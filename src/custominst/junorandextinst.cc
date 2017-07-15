
#include <sst/core/sst_config.h>
#include <sst/core/rng/mersenne.h>

#include "custominst/randaccel/junogenrandresp.h"
#include "custominst/randaccel/junogenrandev.h"
#include "custominst/junorandextinst.h"
#include "custominst/junocustinst.h"

using namespace SST::RNG;
using namespace SST::Juno;

JunoExternalRandInstructionHandler::JunoExternalRandInstructionHandler( Component* owner, Params& params ) :
		JunoCustomInstructionHandler( owner, params ), nextEvID(0) {

	randAccLink = configureLink( "genlink", new Event::Handler<JunoExternalRandInstructionHandler>(
		this, &JunoExternalRandInstructionHandler::handleGenerateResp));
}

JunoExternalRandInstructionHandler::~JunoExternalRandInstructionHandler() {

}

bool JunoExternalRandInstructionHandler::isBusy() {
	return (targetReg > 0);
}

// Return true if the op-code is either RAND or RSEED instructions
// that's all we can process in this unit
bool JunoExternalRandInstructionHandler::canProcessInst( const uint8_t opCode ) {
	return (opCode == JUNO_RAND) || (opCode == JUNO_RSEED);
}

void JunoExternalRandInstructionHandler::handleGenerateResp(SST::Event* ev) {
	cpuOut->verbose(CALL_INFO, 2, 0, "Recv response from random accelerator\n");

	JunoGenerateRandRespEvent* resp = dynamic_cast<JunoGenerateRandRespEvent*>(ev);

	if( NULL == resp ) {
		cpuOut->fatal(CALL_INFO, -1, "Error: event was not a response from rand accelerator\n");
	}

	cpuOut->verbose(CALL_INFO, 2, 0, "Generated random value is: %" PRId64 "\n", resp->getRand());
	registers->writeReg( targetReg, resp->getRand() );
}

void JunoExternalRandInstructionHandler::executeRand( SST::Output* output, const JunoCPUInstruction* inst,
	JunoRegisterFile* regFile ) {

	targetReg = inst->getWriteReg();
	output->verbose(CALL_INFO, 2, 0, "Executing custom RAND instruction: RAND[r%" PRIu8 "]\n", targetReg);

	registers = regFile;
	cpuOut = output;

	JunoGenerateRandEvent* genRand = new JunoGenerateRandEvent(nextEvID++);
	randAccLink->send(genRand);
}

void JunoExternalRandInstructionHandler::executeRandSeed( SST::Output* output, const JunoCPUInstruction* inst,
        JunoRegisterFile* regFile ) {
}

int JunoExternalRandInstructionHandler::execute( SST::Output* output, JunoCPUInstruction* inst,
        JunoRegisterFile* regFile, JunoLoadStoreUnit* loadStoreUnit, uint64_t* pc ) {

	switch( inst->getInstCode() ) {
	case JUNO_RAND:
		executeRand( output, inst, regFile );
		break;

	case JUNO_RSEED:
		break;

	default:
		output->fatal(CALL_INFO, -1, "Error: unknown instruction prefix: %" PRIu8 "\n",
			inst->getInstCode());
		return 1;
	}

	(*pc) = (*pc) + 4;
	return 0;
}
