
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
	int getLiteralIndex(const int64_t checkLit);
	int64_t convertLiteralFromString(const char* litStr);

	std::vector<JunoInstruction*>& getInstructions() {
		return instructions;
	}

	std::vector<uint8_t>& getProgram() {
		return program;
	}

	std::map<std::string, int64_t>& getLabelMap() {
		return labelMap;
	}

protected:
	bool readLine(char* buffer, const int buffLen);
	FILE* progFile;

	std::map<std::string, int64_t> labelMap;
	std::vector<int64_t> literals;
	std::vector<JunoInstruction*> instructions;
	std::vector<uint8_t> program;

};

}
}

#endif
