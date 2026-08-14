#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <string>

using namespace std;

enum class CommandType {
	C_ARITHMETIC,
	C_PUSH,
	C_POP,
	C_LABEL,
	C_GOTO,
	C_IF,
	C_FUNCTION,
	C_RETURN,
	C_CALL
};

class Instruction {
private:
	CommandType commandType;
	string arg1;
	int arg2;

public:
	Instruction(CommandType cmdType, string arg1, int arg2);
	CommandType getCmdType();
	string getArg1();
	int getArg2();
};

#endif