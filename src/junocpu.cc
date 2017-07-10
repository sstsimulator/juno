#include <sst/core/sst_config.h>

#include "junocpu.h"
#include "junocpuinst.h"
#include "junoinstmgr.h"

#include "junoldstdecode.h"
#include "junoldstunit.h"
#include "junojumpctrl.h"
#include "junoalu.h"

#include "instmgr/junofixedprogmgr.h"

using namespace SST::Juno;

JunoCPU::JunoCPU( SST::ComponentId_t id, SST::Params& params ) :
	SST::Component(id) {

	cyclesExecuted = 0;

	int verbosity = params.find<int>("verbose");

	output.init("Juno[" + getName() + ":@p:@t]: ", verbosity, 0, SST::Output::STDOUT);

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

	// Just register a plain clock for this simple example
	std::string cpuClock = params.find<std::string>("clock", "1GHz");
    	registerClock(cpuClock, new SST::Clock::Handler<JunoCPU>(this, &JunoCPU::clockTick));

	// Tell SST to wait until we authorize it to exit
    	registerAsPrimaryComponent();
    	primaryComponentDoNotEndSim();

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

	int maxReg = params.find<int>("registers", "8");
	output.verbose(CALL_INFO, 1, 0, "Creating a register file of %d 64-bit integer registers...\n", maxReg);
	regFile = new JunoRegisterFile(&output, maxReg, &pc, progReader->getDataLength() + progReader->getInstLength() );

	output.verbose(CALL_INFO, 1, 0, "Creating load/store unit...\n");

	ldStUnit = new JunoLoadStoreUnit( &output, mem, regFile );

	output.verbose(CALL_INFO, 1, 0, "Loading operation cycle counts...\n");

	addCycles = params.find<SST::Cycle_t>("cycles-add", 1);
	subCycles = params.find<SST::Cycle_t>("cycles-sub", 1);
	mulCycles = params.find<SST::Cycle_t>("cycles-mul", 1);
	divCycles = params.find<SST::Cycle_t>("cycles-div", 1);
	andCycles = params.find<SST::Cycle_t>("cycles-and", 1);
	xorCycles = params.find<SST::Cycle_t>("cycles-xor", 1);
	orCycles  = params.find<SST::Cycle_t>("cycles-or", 1);

	output.verbose(CALL_INFO, 1, 0, "Initialization done.\n");
}

JunoCPU::~JunoCPU() {
	delete progReader;
	delete regFile;
	delete mem;
}

void JunoCPU::handleEvent( SimpleMem::Request* ev ) {
	output.verbose(CALL_INFO, 4, 0, "Recv response from cache\n");

	if( ev->cmd == Interfaces::SimpleMem::Request::Command::ReadResp ) {
		// Read request needs some special handling
		uint8_t regTarget = ldStUnit->lookupEntry( ev->id );
		int64_t newValue = 0;

		memcpy( (void*) &newValue, &ev->data[0], sizeof(newValue) );

		output.verbose(CALL_INFO, 8, 0, "Response to a read, payload=%" PRId64 ", for reg: %" PRIu8 "\n", newValue, regTarget);

		regFile->writeReg(regTarget, newValue);
	}

	ldStUnit->removeEntry( ev->id );

	output.verbose(CALL_INFO, 4, 0, "Complete cache response handling.\n");
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

	if( 0 == instCyclesLeft ) {
		if( ldStUnit->operationsPending() ) {
			output.verbose(CALL_INFO, 16, 0, "Memory operation pending, no instructions this cycle.\n");
		} else if( instMgr->instReady( pc ) ) {
			output.verbose(CALL_INFO, 2, 0, "Next Instruction, PC=%" PRId64 "...\n", pc);

			JunoCPUInstruction* nextInst = instMgr->getInstruction( pc );
			const uint8_t nextInstOp = nextInst->getInstCode();

			output.verbose(CALL_INFO, 4, 0, "Operation code: %" PRIu8 "\n", nextInstOp);

			switch( nextInstOp ) {
			case JUNO_NOOP :
				pc += 4;
				instCyclesLeft = 1;
				break;

			case JUNO_LOAD :
				executeLoad( output, nextInst, regFile, ldStUnit );
				pc += 4;
				break;

			case JUNO_LOAD_ADDR:
				executeLDA( output, nextInst, regFile, ldStUnit );
				pc += 4;
				break;

			case JUNO_STORE :
				executeStore( output, nextInst, regFile, ldStUnit );
				pc += 4;
				break;

			case JUNO_ADD :
				executeAdd( output, nextInst, regFile );
				pc += 4;
				instCyclesLeft = addCycles;
				break;

			case JUNO_SUB :
				executeSub( output, nextInst, regFile );
				pc += 4;
				instCyclesLeft = subCycles;
				break;

			case JUNO_MUL :
				executeMul( output, nextInst, regFile );
				pc += 4;
				instCyclesLeft = mulCycles;
				break;

			case JUNO_DIV :
				executeDiv( output, nextInst, regFile );
				pc += 4;
				instCyclesLeft = divCycles;
				break;

			case JUNO_AND :
				executeAnd( output, nextInst, regFile );
				pc += 4;
				instCyclesLeft = andCycles;
				break;

			case JUNO_OR :
				executeOr( output, nextInst, regFile );
				pc += 4;
				instCyclesLeft = subCycles;
				break;

			case JUNO_XOR :
				executeXor( output, nextInst, regFile );
				pc += 4;
				instCyclesLeft = xorCycles;
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
	} else {
		output.verbose(CALL_INFO, 4, 0, "CPU still busy (%" PRIu64 " cycles to go.\n", static_cast<uint64_t>(instCyclesLeft));
	}

	if( instCyclesLeft > 0 ) {
		instCyclesLeft--;
	}

	return false;
}
