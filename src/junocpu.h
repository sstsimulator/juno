#ifndef _SST_JUNO_CPU_H
#define _SST_JUNO_CPU_H

#include <sst/core/component.h>
#include <sst/core/elementinfo.h>

#include "junoprogreader.h"
#include "junoregfile.h"
#include "junoinstmgr.h"
#include "junocpuinst.h"

using namespace SST::Juno;

namespace SST {
namespace Juno {

class JunoCPU : public SST::Component {

public:
	JunoCPU( SST::ComponentId_t id, SST::Params& params );
	~JunoCPU();

	void setup();
	void finish();

	bool clockTick( SST::Cycle_t currentCycle );

	SST_ELI_REGISTER_COMPONENT(
		JunoCPU,
		"juno",
		"JunoCPU",
		SST_ELI_ELEMENT_VERSION( 1, 0, 0 ),
		"External programmable CPU for SST",
		COMPONENT_CATEGORY_PROCESSOR
	)

	SST_ELI_DOCUMENT_PARAMS(
		{ "printFrequency", "How frequently to print a message from the component", "5" },
		{ "repeats", "Number of repetitions to make", "10" },
		{ "program", "The assembly file to run.", "" },
		{ "verbose", "Sets the verbosity level of output.", "0" }
	)

private:
	JunoProgramReader* progReader;
	RegisterFile* regFile;
	JunoInstructionMgr* instMgr;
	uint64_t pc;

	uint64_t cyclesExecuted;

	SST::Cycle_t instCyclesLeft;

	SST::Output output;
	SST::Cycle_t printFreq;
	SST::Cycle_t maxRepeats;
	SST::Cycle_t repeats;

};

}
}

#endif

