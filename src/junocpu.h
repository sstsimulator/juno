// Copyright 2013-2023 NTESS. Under the terms
// of Contract DE-NA0003525 with NTESS, the U.S.
// Government retains certain rights in this software.
//
// Copyright (c) 2013-2023, NTESS
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

#include <sst/core/interfaces/stdMem.h>

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
            void handleEvent( StandardMem::Request* ev );

            class MemHandlers : public Interfaces::StandardMem::RequestHandler {
            public:
                friend class JunoCPU;

                MemHandlers(JunoCPU* cpu, SST::Output* out) : Interfaces::StandardMem::RequestHandler(out), cpu(cpu) {}
                virtual ~MemHandlers() {}
                virtual void handle(Interfaces::StandardMem::ReadResp* resp) override;
                virtual void handle(Interfaces::StandardMem::WriteResp* resp) override;

                JunoCPU* cpu;
            };

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
                                    { "clock", "Clock for the CPU", "1GHz" },
                                    { "cycles_add", "Cycles to spend on an ADD operation", "1"},
                                    { "cycles_sub", "Cycles to spend on an SUB operation", "1"},
                                    { "cycles_mul", "Cycles to spend on an MUL operation", "2"},
                                    { "cycles_div", "Cycles to spend on an DIV operation", "6"},
                                    { "cycles_mod", "Cycles to spend on an MOD operation", "6"},
                                    { "cycles_and", "Cycles to spend on an AND operation", "1"},
                                    { "cycles_xor", "Cycles to spend on an XOR operation", "1"},
                                    { "cycles_or",  "Cycles to spend on an OR operation", "1"},
                                    { "cycles_not",  "Cycles to spend on an NOT (bit flip) operation", "1"},
				    { "max_address", "Set a maximum address that memory addresses are allowed to access (debugging mechanism)", "2147483647" }
                                    )

	    SST_ELI_DOCUMENT_STATISTICS(
				   { "cycles", "Cycles the CPU was active", "cycles", 1 },
		  		   { "instructions", "Instructions executed by the CPU", "instructions", 1 },
				   { "mem_reads", "Memory reads issued by the CPU", "instructions", 1 },
				   { "mem_writes", "Memory writes issued by the CPU", "instructions", 1 },
				   { "add_ins_count", "ADD instructions issued by the CPU", "instructions", 1 },
				   { "sub_ins_count", "SUB instructions issued by the CPU", "instructions", 1 },
				   { "mul_ins_count", "MUL instructions issued by the CPU", "instructions", 1 },
				   { "div_ins_count", "DIV instructions issued by the CPU", "instructions", 1 },
				   { "mod_ins_count", "MOD instructions issued by the CPU", "instructions", 1 },
				   { "and_ins_count", "AND instructions issued by the CPU", "instructions", 1 },
				   { "or_ins_count", "OR instructions issued by the CPU", "instructions", 1 },
				   { "xor_ins_count", "XOR instructions issued by the CPU", "instructions", 1 },
				   { "not_ins_count", "NOT instructions issued by the CPU", "instructions", 1 }
				   )

            SST_ELI_DOCUMENT_PORTS(
                                   { "cache_link", "Connects the CPU to the cache", {} }
                                   )

	    SST_ELI_DOCUMENT_SUBCOMPONENT_SLOTS(
                                    {"memory", "Interface to memory system", "SST::Interfaces::StandardMem"},
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

            TimeConverter* timeConverter;

            StandardMem* mem;
            MemHandlers* handlers;
            SST::Cycle_t instCyclesLeft;

            SST::Cycle_t addCycles;
            SST::Cycle_t subCycles;
            SST::Cycle_t divCycles;
            SST::Cycle_t mulCycles;
            SST::Cycle_t andCycles;
            SST::Cycle_t orCycles;
            SST::Cycle_t xorCycles;
	    SST::Cycle_t modCycles;
	    SST::Cycle_t notCycles;

            SST::Output output;

	    Statistic<uint64_t>* statCycles;
	    Statistic<uint64_t>* statInstructions;
            Statistic<uint64_t>* statMemReads;
            Statistic<uint64_t>* statMemWrites;

	    Statistic<uint64_t>* statAddIns;
	    Statistic<uint64_t>* statSubIns;
	    Statistic<uint64_t>* statDivIns;
	    Statistic<uint64_t>* statMulIns;
	    Statistic<uint64_t>* statAndIns;
	    Statistic<uint64_t>* statOrIns;
	    Statistic<uint64_t>* statXorIns;
	    Statistic<uint64_t>* statModIns;
	    Statistic<uint64_t>* statNotIns;

	    std::vector<JunoCustomInstructionHandler*> customHandlers;
        };

    }
}

#endif

