#include <sst/core/sst_config.h>

#include "junocpu.h"
#include "junocpuinst.h"
#include "junoinstmgr.h"

#include "junoalu.h"

#include "instmgr/junofixedprogmgr.h"

using namespace SST::Juno;

JunoCPU::JunoCPU( SST::ComponentId_t id, SST::Params& params ) :
	SST::Component(id), repeats(0) {

	int verbosity = params.find<int>("verbose");

	output.init("Juno-" + getName() + "-> ", verbosity, 0, SST::Output::STDOUT);

	printFreq  = params.find<SST::Cycle_t>("printFrequency", 5);
	maxRepeats = params.find<SST::Cycle_t>("repeats", 10);

	if( ! (printFreq > 0) ) {
		output.fatal(CALL_INFO, -1, "Error: printFrequency must be greater than zero.\n");
	}

	output.verbose(CALL_INFO, 1, 0, "Config: maxRepeats=%" PRIu64 ", printFreq=%" PRIu64 "\n",
		static_cast<uint64_t>(maxRepeats), static_cast<uint64_t>(printFreq));

	// Just register a plain clock for this simple example
    	registerClock("100MHz", new SST::Clock::Handler<JunoCPU>(this, &JunoCPU::clockTick));

	// Tell SST to wait until we authorize it to exit
    	registerAsPrimaryComponent();
    	primaryComponentDoNotEndSim();

	int maxReg = params.find<int>("registers", "8");
	output.verbose(CALL_INFO, 1, 0, "Creating a register file of %d 64-bit integer registers...\n", maxReg);
	regFile = new RegisterFile(maxReg);

	std::string progFile = params.find<std::string>("program", "");

	if( "" == progFile ) {
		output.fatal(CALL_INFO, -1, "Error: program file was not specified, nothing to run!\n");
	}

	output.verbose(CALL_INFO, 1, 0, "Opening program %s ...\n", progFile.c_str());
	FILE* progFileHandle = fopen(progFile.c_str(), "r");

	if( NULL == progFileHandle ) {
		output.fatal(CALL_INFO, -1, "Error: unable to open program: %s\n", progFile.c_str());
	}

	progReader = new JunoProgramReader( progFileHandle , &output );

	fclose(progFileHandle);

	output.verbose(CALL_INFO, 1, 0, "Creating an instruction manager...\n");
	instMgr = new JunoFixedPrgInstMgr( progReader->getBinaryBuffer(), (progReader->getDataLength() + progReader->getInstLength()) );

	instCyclesLeft = 0;
	pc = progReader->getDataLength();

	output.verbose(CALL_INFO, 1, 0, "PC set to: %" PRIu64 "\n", pc);

	output.verbose(CALL_INFO, 1, 0, "Initialization done.\n");
}

JunoCPU::~JunoCPU() {
	delete progReader;
	delete regFile;
}

void JunoCPU::setup() {
	output.verbose(CALL_INFO, 1, 0, "Component is being setup.\n");
}

void JunoCPU::finish() {
	output.verbose(CALL_INFO, 1, 0, "Component is being finished.\n");
}

bool JunoCPU::clockTick( SST::Cycle_t currentCycle ) {

	output.verbose(CALL_INFO, 2, 0, "Cycle: %" PRIu64 "\n", static_cast<uint64_t>(currentCycle));

	if( instCyclesLeft > 0 ) {
		instCyclesLeft--;
	} else {
		if( instMgr->instReady( pc ) ) {
			output.verbose(CALL_INFO, 2, 0, "Next Instruction, PC=%" PRId64 "...\n", pc);

			JunoCPUInstruction* nextInst = instMgr->getInstruction( pc );
			const uint8_t nextInstOp = nextInst->getInstCode();

			output.verbose(CALL_INFO, 4, 0, "Operation code: %" PRIu8 "\n", nextInstOp);

			switch( nextInstOp ) {
			case JUNO_NOOP :
				pc += 4;
				break;

			case JUNO_LOAD :
				pc += 4;
				break;

			case JUNO_STORE :
				pc += 4;
				break;

			case JUNO_ADD :
				executeAdd( output, nextInst, regFile );
				pc += 4;
				break;

			case JUNO_SUB :
				pc += 4;
				break;

			case JUNO_MUL :
				pc += 4;
				break;

			case JUNO_DIV :
				pc += 4;
				break;

			case JUNO_AND :
				pc += 4;
				break;

			case JUNO_OR :
				pc += 4;
				break;

			case JUNO_XOR :
				pc += 4;
				break;

			case JUNO_NOT :
				pc += 4;
				break;

			case JUNO_PCR_JUMP:
				pc += 4;
				break;

			case JUNO_PCR_JUMP_ZERO:
				pc += 4;
				break;

			case JUNO_PCR_JUMP_LTZ:
				pc += 4;
				break;

			case JUNO_PCR_JUMP_GTZ:
				pc += 4;
				break;

			case JUNO_HALT :
				primaryComponentOKToEndSim();
				return true;
			default:
				break;

			}

		}
	}

	return false;
}
