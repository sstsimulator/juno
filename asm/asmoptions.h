
#ifndef _H_JUNO_ASM_OPTIONS
#define _H_JUNO_ASM_OPTIONS

#include <cstdio>
#include <string>

namespace SST {
namespace Juno {
namespace Assembler {

class AssemblerOptions {

public:
	AssemblerOptions() {
		inputFilePath = "-";
		outputFilePath = "-";

		inputFile = stdin;
		outputFile = stdout;
	}

	~AssemblerOptions() {
		if( NULL != inputFile && inputFile != stdin ) {
			fclose(inputFile);
		}

		if( NULL != outputFile && outputFile != stdout ) {
			fclose(outputFile);
		}
	}

	FILE* getInputFile() {
		return inputFile;
	}

	FILE* getOutputFile() {
		return outputFile;
	}

	void setInputFilePath(const std::string& path) {
		inputFilePath = path;

		if( NULL != inputFile && inputFile != stdin ) {
			fclose( inputFile );
		}

		if( path == "-" ) {
			inputFile = stdin;
		} else {
			inputFile = fopen( path.c_str(), "r" );
		}
	}

	void setOutputFilePath(const std::string& path) {
		outputFilePath = path;

		if( NULL != outputFile ) {
			fclose( outputFile );
		}

		if( path == "-" && outputFile != stdout ) {
			outputFile = stdout;
		} else {
			outputFile = fopen( path.c_str(), "w" );
		}
	}

	std::string getInputFilePath() {
		return inputFilePath;
	}

	std::string getOutputFilePath() {
		return outputFilePath;
	}

protected:
	FILE* inputFile;
	FILE* outputFile;
	std::string inputFilePath;
	std::string outputFilePath;

};

}
}
}

#endif
