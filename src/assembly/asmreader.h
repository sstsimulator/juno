
#ifndef _H_SST_JUNO_ASM_READER_
#define _H_SST_JUNO_ASM_READER_

#include "asminst.h"

namespace SST {
namespace Juno {

class AssemblyReader {

public:
	AssemblyReader(const char* progPath);
	~AssemblyReader();

	void assemble();

protected:
	bool readLine(char* buffer, const int buffLen);
	FILE* progFile;
	std::vector<uint8_t> program;

};

}
}

#endif
