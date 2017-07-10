#include <sst/core/sst_config.h>

#include "junocpu.h"
#include "junocpuinst.h"
#include "junoinstmgr.h"

#include "junoldstdecode.h"
#include "junojumpctrl.h"
#include "junoalu.h"

#include "instmgr/junofixedprogmgr.h"

using namespace SST::Juno;

JunoCPU::JunoCPU( SST::ComponentId_t id, SST::Params& params ) :
	SST::Component(id), repeats(0) {

	cyclesExecuted = 0;

	int verbosity = params.find<int>("verbose");

	output.init("Juno-" + getName() + "-> ", verbosity, 0, SST::Output::STDOUT);

	printFreq  = params.find<SST::Cycle_t>("printFrequency", 5);
	maxRepeats = params.find<SST::Cycle_t>("repeats", 10);

	if( ! (printFreq > 0) ) {
		output.fatal(CALL_INFO, -1, "Error: printFrequency must be greater than zero.\n");
	}

	std::string memIFace = params.find<std::string>("meminterface", "memHierarchy.memInterface");
	output.verbose(CALL_INFO, 1, 0, "Loading memory interface: %s ...\n", memIFace.c_str());

	Params interfaceParams = params.find_prefix_params("meminterface.");

	mem = dynamic_cast<SimpleMem*>( loadModuleWithComponent(memIFace, this, interfaceParams) );

	if( NULL == mem ) {
		output.fatal(CALL_INFO, -1, "Error: unable to load %s memory interface.\n", memIFace.c_str());
	} else {
		output.verbose(CALL_INFO, 1, 0, "Successfully loaded memory interface.\n");
	}

	bool init_link_success = mem->initialize("cache_link", new SimpleMem::Handler<JunoCPU>(this, &JunoCPU::handleEvent) );

	if( init_link_success ) {
		output.verbose(CALL_INFO, 1, 0, "Cache link (via SimpleMem) initialized successfully\n");
	} else {
		output.fatal(CALL_INFO, -1, "Cache link was not initialized successfully\n");
	}

	output.verbose(CALL_INFO, 1, 0, "Config: maxRepeats=%" PRIu64 ", printFreq=%" PRIu64 "\n",
		static_cast<uint64_t>(maxRepeats), static_cast<uint64_t>(printFreq));

	// Just register a plain clock for this simple example
	std::string cpuClock = params.find<std::string>("clock", "1GHz");
    	registerClock(cpuClock, new SST::Clock::Handler<JunoCPU>(this, &JunoCPU::clockTick));

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
	delete mem;
}

void JunoCPU::handleEvent( SimpleMem::Request* ev ) {
	output.verbose(CALL_INFO, 2, 0, "Recv Event from Cache.\n");
}

void JunoCPU::init( unsigned int phase ) {
	if( 0 == phase ) {
		const int initLen = progReader->getDataLength() + progReader->getInstLength();

		std::vector<uint8_t> exeImage;
		exeImage.reserve( initLen );

		for( int i = 0; i < initLen; ++i ) {
			exeImage.push_back( progReader->getBinaryBuffer()[i] );
		}

		SimpleMem::Request* writeExe = new SimpleMem::Request(SimpleMem::Request::Write, 0, exeImage.size(), exeImage);
		output.verbose(CALL_INFO, 1, 0, "Sending initialization data to memory...\n");

		mem->sendInitData(writeExe);

		output.verbose(CALL_INFO, 1, 0, "Initialization data sent.\n");
	}
}

void JunoCPU::setup() {
	output.verbose(CALL_INFO, 1, 0, "Component is being setup.\n");
}

void JunoCPU::finish() {
	output.verbose(CALL_INFO, 1, 0, "Component is being finished (executed: %" PRId64 " cycles)\n", cyclesExecuted);
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

			case JUNO_LOAD_ADDR:
				executeLDA( output, nextInst, regFile, mem );
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
				executeSub( output, nextInst, regFile );
				pc += 4;
				break;

			case JUNO_MUL :
				executeMul( output, nextInst, regFile );
				pc += 4;
				break;

			case JUNO_DIV :
				executeDiv( output, nextInst, regFile );
				pc += 4;
				break;

			case JUNO_AND :
				executeAnd( output, nextInst, regFile );
				pc += 4;
				break;

			case JUNO_OR :
				executeOr( output, nextInst, regFile );
				pc += 4;
				break;

			case JUNO_XOR :
				executeXor( output, nextInst, regFile );
				pc += 4;
				break;

			case JUNO_NOT :
				pc += 4;
				break;

			case JUNO_PCR_JUMP_ZERO:
				executeJumpZero( output, nextInst, regFile, &pc );
				break;

			case JUNO_PCR_JUMP_LTZ:
				executeJumpLTZ( output, nextInst, regFile, &pc );
				break;

			case JUNO_PCR_JUMP_GTZ:
				executeJumpGTZ( output, nextInst, regFile, &pc );
				break;

			case JUNO_HALT :
				primaryComponentOKToEndSim();
				return true;

			default:
				fprintf(stderr, "ERROR: Unknown instruction encountered.\n");
				exit(-1);
				break;

			}

		}
	}

	return false;
}
