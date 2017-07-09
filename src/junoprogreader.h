
#ifndef _H_SST_JUNO_PROG_READER
#define _H_SST_JUNO_PROG_READER

#include <cstdio>
#include <cinttypes>
#include <cstdlib>

namespace SST {
namespace Juno {

class JunoProgramReader {

public:
	JunoProgramReader( FILE* appBinary, SST::Output* output ) {
		fread( (void*) &binVersion, sizeof(binVersion), 1, appBinary );
		fread( (void*) &dataLen, sizeof(dataLen), 1, appBinary );
		fread( (void*) &instLen, sizeof(instLen), 1, appBinary );

		output->verbose(CALL_INFO, 1, 0, "Application Version:      %" PRId64 "\n", binVersion);
		output->verbose(CALL_INFO, 1, 0, "Data Length:              %" PRIu64 "\n", dataLen);
		output->verbose(CALL_INFO, 1, 0, "Inst Length:              %" PRIu64 "\n", instLen);

		programBinary = (char*) malloc( sizeof(char) * (dataLen + instLen));

		fread( (void*) programBinary, sizeof(char), (dataLen + instLen), appBinary );
	}

	~JunoProgramReader() {
		free( programBinary );
	}

	int64_t getBinaryVersion() {
		return binVersion;
	}

	uint64_t getDataLength() {
		return dataLen;
	}

	uint64_t getInstLength() {
		return instLen;
	}

	char* getBinaryBuffer() {
		return programBinary;
	}

protected:
	int64_t  binVersion;
	uint64_t dataLen;
	uint64_t instLen;
	char* programBinary;


};

}
}

#endif
