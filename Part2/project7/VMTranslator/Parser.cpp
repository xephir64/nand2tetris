#include "Parser.h"

Parser::Parser(string command) {
	this->currentIndex = 0;
	string s;
	stringstream ss(command);
	while (getline(ss, s)) {
		if (!s.empty() && s.back() == '\r') {
			s.pop_back();
		}
		if (s.rfind("//", 0) == 0) continue;
		if (s.empty()) continue;

		std::istringstream iss(s);
		std::string token;
		std::vector<string> instruction;

		while (iss >> token) {
			instruction.push_back(token);
		}

		CommandType cmdType = getCmdType(instruction.at(0));
		string arg1;
		int arg2 = -1;
		if (cmdType == CommandType::C_ARITHMETIC) arg1 = instruction.at(0);
		else arg1 = instruction.at(1);
		if (cmdType == CommandType::C_PUSH || cmdType == CommandType::C_POP || cmdType == CommandType::C_FUNCTION || cmdType == CommandType::C_CALL)
			arg2 = std::stoi(instruction.at(2));
		instructions.push_back(Instruction(cmdType, arg1, arg2));
	}
}

bool Parser::hasMoreCommands() {
	return currentIndex < this->instructions.size();
}

void Parser::advance() {
	if (currentIndex < this->instructions.size()) currentIndex++;
	else currentIndex = 0;
}

CommandType Parser::commandType() {
	return this->instructions.at(currentIndex).getCmdType();
}

string Parser::arg1() {
	return this->instructions.at(currentIndex).getArg1();

}

int Parser::arg2() {
	return this->instructions.at(currentIndex).getArg2();
}

CommandType Parser::getCmdType(string command) {
	if (command.find("push") != string::npos) {
		return CommandType::C_PUSH;
	}
	else if (command.find("pop") != string::npos) {
		return CommandType::C_POP;
	}
	else if (command.find("label") != string::npos) {
		return CommandType::C_LABEL;
	}
	else if (command.find("goto") != string::npos) {
		return CommandType::C_GOTO;
	}
	else if (command.find("if-goto") != string::npos) {
		return CommandType::C_IF;
	}
	else if (command.find("function") != string::npos) {
		return CommandType::C_FUNCTION;
	}
	else if (command.find("return") != string::npos) {
		return CommandType::C_RETURN;
	}
	else if (command.find("call") != string::npos) {
		return CommandType::C_CALL;
	}
	else {
		return CommandType::C_ARITHMETIC;
	}
}
