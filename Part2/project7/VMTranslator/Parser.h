#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <regex>

#include "Instruction.h"

using namespace std;

class Parser {
public:
	Parser(string command);
	bool hasMoreCommands();
	void advance();
	CommandType commandType();
	string arg1();
	int arg2();
private:
	CommandType getCmdType(string command);
	int currentIndex;
	vector<Instruction> instructions;
};

#endif