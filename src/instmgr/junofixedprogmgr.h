
#ifndef _H_SST_JUNO_FIXED_PRG_MGR
#define _H_SST_JUNO_FIXED_PRG_MGR

#include <cinttypes>
#include "junoinstmgr.h"

namespace SST {
namespace Juno {

class JunoFixedPrgInstMgr : public JunoInstructionMgr {

public:
	JunoFixedPrgInstMgr( const char* buff, const uint64_t length ) :
		JunoInstructionMgr(), maxLen(length) {

		buffer = (char*) malloc( sizeof(char) * maxLen );
		memcpy( buffer, buff, maxLen );
	}
	~JunoFixedPrgInstMgr() {}

	bool instReady( const uint64_t addr ) {
		return true;
	}

	JunoCPUInstruction* getInstruction( const uint64_t addr ) {
		int32_t instCode = 0;

		memcpy( (void*) &instCode, &buffer[addr], sizeof(instCode) );
		JunoCPUInstruction* inst = new JunoCPUInstruction( instCode);

		return inst;
	}

protected:
	char* buffer;
	uint64_t maxLen;

};

}
}

#endif
