// Copyright 2013-2024 NTESS. Under the terms
// of Contract DE-NA0003525 with NTESS, the U.S.
// Government retains certain rights in this software.
//
// Copyright (c) 2013-2024, NTESS
// All rights reserved.
//
// Portions are copyright of other developers:
// See the file CONTRIBUTORS.TXT in the top level directory
// of the distribution for more information.
//
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.


#ifndef _SST_JUNO_CPU_H
#define _SST_JUNO_CPU_H

#include <sst/core/sst_config.h>
#include <sst/core/component.h>
#include <sst/core/elementinfo.h>

#include <sst/core/interfaces/simpleMem.h>

#include "junoldstunit.h"
#include "junoprogreader.h"
#include "junoregfile.h"
#include "junoinstmgr.h"
#include "junocpuinst.h"

#include "custominst/junocustinst.h"

using namespace SST::Interfaces;
using namespace SST::Juno;

namespace SST {
    namespace Juno {

        class JunoCPU : public SST::Component {

        public:
            JunoCPU( SST::ComponentId_t id, SST::Params& params );
            ~JunoCPU();

            void setup();
            void finish();

            void init( unsigned int phase );

            bool clockTick( SST::Cycle_t currentCycle );
            void handleEvent( SimpleMem::Request* ev );

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
                                    { "verbose", "Sets the verbosity level of output.", "0" },
                                    { "cycles-add", "Cycles to spend on an ADD operation", "1"},
                                    { "cycles-sub", "Cycles to spend on an SUB operation", "1"},
                                    { "cycles-mul", "Cycles to spend on an MUL operation", "2"},
                                    { "cycles-div", "Cycles to spend on an DIV operation", "6"},
                                    { "cycles-mod", "Cycles to spend on an MOD operation", "6"},
                                    { "cycles-and", "Cycles to spend on an AND operation", "1"},
                                    { "cycles-xor", "Cycles to spend on an XOR operation", "1"},
                                    { "cycles-or",  "Cycles to spend on an OR operation", "1"},
				    { "max-address", "Set a maximum address that memory addresses are allowed to access (debugging mechanism)", "2147483647" }
                                    )

            SST_ELI_DOCUMENT_PORTS(
                                   { "cache_link", "Connects the CPU to the cache", {} }
                                   )

	    SST_ELI_DOCUMENT_SUBCOMPONENT_SLOTS(
        		{"customhandler", "Holds customer instruction handlers",
				"SST::Juno::CustomInstructionHandler" }
    		)

        private:
            JunoProgramReader* progReader;
            JunoRegisterFile* regFile;
            JunoInstructionMgr* instMgr;
            JunoLoadStoreUnit* ldStUnit;
	    int handlerCount;
            uint64_t pc;

            SimpleMem* mem;

            SST::Cycle_t instCyclesLeft;

            SST::Cycle_t addCycles;
            SST::Cycle_t subCycles;
            SST::Cycle_t divCycles;
            SST::Cycle_t mulCycles;
            SST::Cycle_t andCycles;
            SST::Cycle_t orCycles;
            SST::Cycle_t xorCycles;
	    SST::Cycle_t modCycles;

            SST::Output output;

	    std::vector<JunoCustomInstructionHandler*> customHandlers;
        };

    }
}

#endif

