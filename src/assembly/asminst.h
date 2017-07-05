
#ifndef _H_SST_JUNO_ASM_INST_
#define _H_SST_JUNO_ASM_INST_

#include <vector>
#include <cstdint>
#include <cinttypes>
#include <cstring>
#include <string>

namespace SST {
namespace Juno {

enum JunoOperandType {
	REGISTER_OPERAND,
	MEMORY_OPERAND,
	LITERAL_OPERAND
};

class JunoOperand {
public:
	JunoOperand() {}
	virtual JunoOperandType getType() = 0;
};

class JunoRegisterOperand : public JunoOperand {
	#define JUNO_MAX_REGISTER_DIGITS 6
public:
	JunoRegisterOperand(const uint16_t regst) :
		JunoOperand(), reg(regst) {}
	JunoRegisterOperand(const char* regst) :
		JunoOperand() {

		if(strlen(regst) == 0) {
			fprintf(stderr, "Error: register number is empty.\n");
			exit(-1);
		}

		if( strlen(regst) > JUNO_MAX_REGISTER_DIGITS || strlen(regst) == 1 ) {
			fprintf(stderr, "Error: register number (%s) is badly formed.\n", regst);
			exit(-1);
		}

		if( ! (regst[0] == 'r' || regst[0] == 'R') ) {
			fprintf(stderr, "Error: register badly formed - must start with r. (%s)\n", regst);
			exit(-1);
		}

		char regNum[JUNO_MAX_REGISTER_DIGITS];
		for(int i = 1; i < std::max(static_cast<size_t>(JUNO_MAX_REGISTER_DIGITS), strlen(regst)); ++i) {
			regNum[i-1] = regst[i];
		}

		reg = static_cast<uint16_t>( std::atoi(regNum) );
		printf("Register: %" PRIu16 "\n", reg);
	}
	JunoOperandType getType() { return REGISTER_OPERAND; }
	uint16_t getRegister() { return reg; }

protected:
	uint16_t reg;
};

class JunoLiteralOperand : public JunoOperand {
public:
	JunoLiteralOperand(const int64_t literal) :
		JunoOperand(), val(literal) {}

	JunoLiteralOperand(const char* litStr) :
		JunoOperand() {

		if(strlen(litStr) == 0) {
			fprintf(stderr, "Error: literal value is empty.\n");
			exit(-1);
		}

		if(strlen(litStr) == 1) {
			fprintf(stderr, "Error: literal value is not long enough!\n");
			exit(-1);
		}

		if( '$' != litStr[0] ) {
			fprintf(stderr, "Error: literal value does not start with a $\n");
			exit(-1);
		}

		char* literalValue = (char*) malloc( sizeof(char) * 128 );
		for(int i = 1; i < 128; ++i) {
			literalValue[i-1] = litStr[i];
		}

		val = static_cast<int64_t>( std::atoll(literalValue) );
		printf("Literal: %" PRId64 "\n", val);
	}

	JunoOperandType getType() { return LITERAL_OPERAND; }
	int64_t getLiteral() { return val; }

protected:
	int64_t val;
};

class JunoMemoryOperand : public JunoOperand {
public:
	JunoMemoryOperand(const uint64_t address) :
		JunoOperand(), addr(address) {}
	JunoMemoryOperand(const char* addrStr) :
		JunoOperand() {

		char* num_end;
		addr = static_cast<uint64_t>( std::strtoull(addrStr, &num_end, 10) );
	}
	uint64_t getAddress() { return addr; }
	JunoOperandType getType() { return MEMORY_OPERAND; }

protected:
	uint64_t addr;
};

class JunoInstruction {

public:
	JunoInstruction(const char* instMnu) :
		inst(instMnu) {}
	~JunoInstruction() {}

	void addOperand(JunoOperand* op) {
		operands.push_back(op);
	}

	int countOperands() {
		return static_cast<int>( operands.size() );
	}

	JunoOperand* getOperand(const int i) {
		return operands.at(i);
	}

	std::string getInstCode() { return inst; }

protected:
	std::string inst;
	std::vector<JunoOperand*> operands;

};

}
}

#endif
