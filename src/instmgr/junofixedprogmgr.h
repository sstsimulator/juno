
#ifndef _H_SST_JUNO_FIXED_PRG_MGR
#define _H_SST_JUNO_FIXED_PRG_MGR

#include <cinttypes>
#include "junoinstmgr.h"

namespace SST {
namespace Juno {

class JunoFixedPrgInstMgr : public JunoInstructionMgr {

public:
	JunoFixedPrgInstMgr( std::vector<JunoCPUInstruction*> program ) : JunoInstructionMgr() {}
	~JunoFixedPrgInstMgr() {}

	bool instReady( const uint64_t addr ) {
		return true;
	}

	JunoCPUInstruction* getInstruction( const uint64_t addr ) {
		return prog.at(addr);
	}

protected:
	std::vector<JunoCPUInstruction*> prog;

};

}
}

#endif
