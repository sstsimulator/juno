
#ifndef _H_SST_JUNO_INST_MGR
#define _H_SST_JUNO_INST_MGR

#include "junocpuinst.h"

namespace SST {
    namespace Juno {
        
        class JunoInstructionMgr {
            
        public:
            JunoInstructionMgr() {}
            ~JunoInstructionMgr() {}
            
            virtual JunoCPUInstruction* getInstruction( const uint64_t addr ) = 0;
            virtual bool instReady( const uint64_t addr ) = 0;
            
        };
        
    }
}

#endif
