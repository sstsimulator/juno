
#ifndef _H_SST_JUNO_ALU
#define _H_SST_JUNO_ALU

#include <cinttypes>

#include "junocpuinst.h"
#include "junoregfile.h"

namespace SST {
    namespace Juno {
        
        void executeAdd( SST::Output& output, JunoCPUInstruction* inst, JunoRegisterFile* regFile ) {
            const uint8_t opLeft    = inst->getReadReg1();
            const uint8_t opRight   = inst->getReadReg2();
            const uint8_t resultReg = inst->getWriteReg();
            
            const int64_t valLeft   = regFile->readReg(opLeft);
            const int64_t valRight  = regFile->readReg(opRight);
            
            const int64_t result    = valLeft + valRight;
            
            output.verbose(CALL_INFO, 4, 0, "ADD[r%3" PRIu8 ", r%3" PRIu8 ", res=r%3" PRIu8 "]: (%" PRId64 " + %" PRId64 " = %" PRId64 ")\n",
                           opLeft, opRight, resultReg, valLeft, valRight, result);
            
            regFile->writeReg( resultReg, result );
        };
        
        void executeSub( SST::Output& output, JunoCPUInstruction* inst, JunoRegisterFile* regFile ) {
            const uint8_t opLeft    = inst->getReadReg1();
            const uint8_t opRight   = inst->getReadReg2();
            const uint8_t resultReg = inst->getWriteReg();
            
            const int64_t valLeft   = regFile->readReg(opLeft);
            const int64_t valRight  = regFile->readReg(opRight);
            
            const int64_t result    = valLeft - valRight;
            
            output.verbose(CALL_INFO, 4, 0, "SUB[r%3" PRIu8 ", r%3" PRIu8 ", res=r%3" PRIu8 "]: (%" PRId64 " - %" PRId64 " = %" PRId64 ")\n",
                           opLeft, opRight, resultReg, valLeft, valRight, result);
            
            regFile->writeReg( resultReg, result );
        };
        
        void executeDiv( SST::Output& output, JunoCPUInstruction* inst, JunoRegisterFile* regFile ) {
            const uint8_t opLeft    = inst->getReadReg1();
            const uint8_t opRight   = inst->getReadReg2();
            const uint8_t resultReg = inst->getWriteReg();
            
            const int64_t valLeft   = regFile->readReg(opLeft);
            const int64_t valRight  = regFile->readReg(opRight);
            
            const int64_t result    = valLeft / valRight;
            
            output.verbose(CALL_INFO, 4, 0, "DIV[r%3" PRIu8 ", r%3" PRIu8 ", res=r%3" PRIu8 "]: (%" PRId64 " / %" PRId64 " = %" PRId64 ")\n",
                           opLeft, opRight, resultReg, valLeft, valRight, result);
            
            regFile->writeReg( resultReg, result );
        };
        
        void executeMul( SST::Output& output, JunoCPUInstruction* inst, JunoRegisterFile* regFile ) {
            const uint8_t opLeft    = inst->getReadReg1();
            const uint8_t opRight   = inst->getReadReg2();
            const uint8_t resultReg = inst->getWriteReg();
            
            const int64_t valLeft   = regFile->readReg(opLeft);
            const int64_t valRight  = regFile->readReg(opRight);
            
            const int64_t result    = valLeft * valRight;
            
            output.verbose(CALL_INFO, 4, 0, "MUL[r%3" PRIu8 ", r%3" PRIu8 ", res=r%3" PRIu8 "]: (%" PRId64 " * %" PRId64 " = %" PRId64 ")\n",
                           opLeft, opRight, resultReg, valLeft, valRight, result);
            
            regFile->writeReg( resultReg, result );
        };
        
        void executeAnd( SST::Output& output, JunoCPUInstruction* inst, JunoRegisterFile* regFile ) {
            const uint8_t opLeft    = inst->getReadReg1();
            const uint8_t opRight   = inst->getReadReg2();
            const uint8_t resultReg = inst->getWriteReg();
            
            const int64_t valLeft   = regFile->readReg(opLeft);
            const int64_t valRight  = regFile->readReg(opRight);
            
            const int64_t result    = valLeft & valRight;
            
            output.verbose(CALL_INFO, 4, 0, "AND[r%3" PRIu8 ", r%3" PRIu8 ", res=r%3" PRIu8 "]: (%" PRId64 " & %" PRId64 " = %" PRId64 ")\n",
                           opLeft, opRight, resultReg, valLeft, valRight, result);
            
            regFile->writeReg( resultReg, result );
        };
        
        void executeOr( SST::Output& output, JunoCPUInstruction* inst, JunoRegisterFile* regFile ) {
            const uint8_t opLeft    = inst->getReadReg1();
            const uint8_t opRight   = inst->getReadReg2();
            const uint8_t resultReg = inst->getWriteReg();
            
            const int64_t valLeft   = regFile->readReg(opLeft);
            const int64_t valRight  = regFile->readReg(opRight);
            
            const int64_t result    = valLeft | valRight;
            
            output.verbose(CALL_INFO, 4, 0, "OR[r%3" PRIu8 ", r%3" PRIu8 ", res=r%3" PRIu8 "]: (%" PRId64 " | %" PRId64 " = %" PRId64 ")\n",
                           opLeft, opRight, resultReg, valLeft, valRight, result);
            
            regFile->writeReg( resultReg, result );
        };
        
        void executeXor( SST::Output& output, JunoCPUInstruction* inst, JunoRegisterFile* regFile ) {
            const uint8_t opLeft    = inst->getReadReg1();
            const uint8_t opRight   = inst->getReadReg2();
            const uint8_t resultReg = inst->getWriteReg();
            
            const int64_t valLeft   = regFile->readReg(opLeft);
            const int64_t valRight  = regFile->readReg(opRight);
            
            const int64_t result    = valLeft ^ valRight;
            
            output.verbose(CALL_INFO, 4, 0, "XOR[r%3" PRIu8 ", r%3" PRIu8 ", res=r%3" PRIu8 "]: (%" PRId64 " ^ %" PRId64 " = %" PRId64 ")\n",
                           opLeft, opRight, resultReg, valLeft, valRight, result);
            
            regFile->writeReg( resultReg, result );
        };
        
        
        
        
    }
}

#endif
