#include "Instruction.h"


Instruction::Instruction(CommandType cmdType, string arg1, int arg2) {
	this->commandType = cmdType;
	this->arg1 = arg1;
	this->arg2 = arg2;
}

CommandType Instruction::getCmdType() {
	return this->commandType;
}

string Instruction::getArg1() {
	return this->arg1;
}

int Instruction::getArg2() {
	return this->arg2;
}