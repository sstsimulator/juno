
#ifndef _H_JUNO_ASM_OPTIONS
#define _H_JUNO_ASM_OPTIONS

#include <cstdio>
#include <string>

namespace SST {
namespace Juno {
namespace Assembler {

class AssemblerOptions {

public:
	AssemblerOptions(const int argc, char* argv[]) {
		outputFilePath = "program.bin";
		inputFilePath.clear();

		for(int i = 1; i < argc; ++i) {
			if( 0 == strcmp("-o", argv[i]) ) {
				if( (i+1) < argc ) {
					outputFilePath.clear();
					outputFilePath.append( argv[i+1] );
					i = i + 1;
				} else {
					fprintf(stderr, "Error: specified -o but did not provide a path.\n");
					exit(-1);
				}
			} else if( 0 == strcmp("-i", argv[i]) ) {
				if( (i+1) < argc ) {
					inputFilePath.clear();
					inputFilePath.append( argv[i+1] );
					i = i + 1;
				} else {
					fprintf(stderr, "Error: specified -i but did not provide an input path.\n");
					exit(-1);
				}
			} else if(  0 == strcmp("-help", argv[i]) ||
						0 == strcmp("--help", argv[i]) ||
						0 == strcmp("-h", argv[i]) ) {

				printf("sst-juno-asm [-i <input file>] [-o <output file>]\n");
				printf("\n");
				printf("<input file>   File to read in, if not specified stdin\n");
				printf("<output file>  File to write to, if not specified program.bin\n");
				printf("\n");
				exit(0);
			} else {
				fprintf(stderr, "Unknown option: \"%s\"\n", argv[i]);
				exit(-1);
			}
		}

		if( "" == inputFilePath ) {
			inputFile = stdin;
		} else {
			inputFile = fopen( inputFilePath.c_str(), "r" );
			inputFilePath = "-";
		}

		if( "" == outputFilePath ) {
			outputFile = stdout;
		} else {
			outputFile = fopen( outputFilePath.c_str(), "w" );
			outputFilePath = "-";
		}
	}

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
