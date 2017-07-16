
#ifndef _H_SST_JUNO_ASM_READER_
#define _H_SST_JUNO_ASM_READER_

#include "asmop.h"
#include "asmprogram.h"
#include "asmoptions.h"

namespace SST {
namespace Juno {
namespace Assembler {

class AssemblyReader {

public:
	AssemblyReader(AssemblerOptions* options);
	~AssemblyReader();

	AssemblyProgram* assemble();
	int getLiteralIndex(const int64_t checkLit);
	int64_t convertLiteralFromString(const char* litStr);

protected:
	AssemblerOptions* options;
	bool readLine(char* buffer, const int buffLen);

};

}
}
}

#endif
