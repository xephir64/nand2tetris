#include "CodeWriter.h"
#include <filesystem>
#include <stdexcept>

using std::string;

CodeWriter::CodeWriter(string filename)
    : static_name(std::filesystem::path(filename).stem().string()),
      count_label(0) 
{
    output_file.open(filename);
}

void CodeWriter::writeArithmetic(string command) {
	output_file << "// " << command << endl;
	if (command == "neg") {
		writeAsm(
			"@SP", "AM=M-1", "D=M", "M=-D", "@SP", "M=M+1"
		);
	}
	if (command == "not") {
		writeAsm(
			"@SP", "AM=M-1", "D=M", "M=!D", "@SP", "M=M+1"
		);
	}
	if (command == "add") {
		writeAsm(
			"@SP", "AM=M-1", "D=M", "@SP", "AM=M-1", "M=D+M", "@SP", "M=M+1"
		);
	}
	if (command == "sub") {
		writeAsm(
			"@SP", "AM=M-1", "D=M", "@SP", "AM=M-1", "M=M-D", "@SP", "M=M+1"
		);
	}
	if (command == "or") {
		writeAsm(
			"@SP", "AM=M-1", "D=M", "@SP", "AM=M-1", "M=D|M", "@SP", "M=M+1"
		);
	}
	if (command == "and") {
		writeAsm(
			"@SP", "AM=M-1", "D=M", "@SP", "AM=M-1", "M=D&M", "@SP", "M=M+1"
		);
	}
	if (command == "lt") {
		writeAsm(
			"@SP",
			"AM=M-1",
			"D=M",
			"@SP",
			"AM=M-1",
			"D=M-D",
			"@LT_TRUE_" + to_string(count_label),
			"D;JLT",
			"@SP",
			"A=M",
			"M=0",
			"@LT_END_" + to_string(count_label),
			"0;JMP",

			"(LT_TRUE_" + to_string(count_label) + ")",
			"@SP",
			"A=M",
			"M=-1",

			"(LT_END_" + to_string(count_label) + ")",
			"@SP",
			"M=M+1"
		);
		count_label++;
	}
	if (command == "gt") {
		writeAsm(
			"@SP",
			"AM=M-1",
			"D=M",
			"@SP",
			"AM=M-1",
			"D=M-D",
			"@GT_TRUE_" + to_string(count_label),
			"D;JGT",
			"@SP",
			"A=M",
			"M=0",
			"@GT_END_" + to_string(count_label),
			"0;JMP",

			"(GT_TRUE_" + to_string(count_label) + ")",
			"@SP",
			"A=M",
			"M=-1",

			"(GT_END_" + to_string(count_label) + ")",
			"@SP",
			"M=M+1"
		);
		count_label++;
	}
	if (command == "eq") {
		writeAsm(
			"@SP",
			"AM=M-1",
			"D=M",
			"@SP",
			"AM=M-1",
			"D=M-D",
			"@EQ_TRUE_" + to_string(count_label),
			"D;JEQ",
			"@SP",
			"A=M",
			"M=0",
			"@EQ_END_" + to_string(count_label),
			"0;JMP",

			"(EQ_TRUE_" + to_string(count_label) + ")",
			"@SP",
			"A=M",
			"M=-1",

			"(EQ_END_" + to_string(count_label) + ")",
			"@SP",
			"M=M+1"
		);
		count_label++;
	}
}

void CodeWriter::writePushPop(CommandType command, string segment, int index) {
	if (command == CommandType::C_PUSH) {
		output_file << "// " << "push " << segment << " " << index << endl;
		if (segment == "constant") {
			writeAsm(
				"@" + to_string(index),
				"D=A",
				"@SP",
				"A=M",
				"M=D",
				"@SP",
				"M=M+1"
			);
		}
		if (segment == "local")	this->writePushSegment(Segment::Local, index);
		if (segment == "argument") this->writePushSegment(Segment::Argument, index);
		if (segment == "this") this->writePushSegment(Segment::This, index);
		if (segment == "that") this->writePushSegment(Segment::That, index);
		if (segment == "temp") this->writePushTemp(index);
		if (segment == "pointer") this->writePushPointer(index);
		if (segment == "static") this->writePushStatic(index);
	}
	else if (command == CommandType::C_POP) {
		this->output_file << "// " << "pop " << segment << " " << index << endl;
		if (segment == "local")	this->writePopSegment(Segment::Local, index);
		if (segment == "argument") this->writePopSegment(Segment::Argument, index);
		if (segment == "this") this->writePopSegment(Segment::This, index);
		if (segment == "that") this->writePopSegment(Segment::That, index);
		if (segment == "temp") this->writePopTemp(index);
		if (segment == "pointer") this->writePopPointer(index);
		if (segment == "static") this->writePopStatic(index);
	}
}

void CodeWriter::writePushSegment(Segment segment, int index) {
	writeAsm(
		"@" + to_string(index),
		"D=A",
		"@" + segmentName(segment),
		"A=D+M",
		"D=M",
		"@SP",
		"A=M",
		"M=D",
		"@SP",
		"M=M+1"
		);
}

void CodeWriter::writePushTemp(int index) {
	writeAsm(
		"@" + to_string(TEMP + index),
		"D=M",
		"@SP",
		"A=M",
		"M=D",
		"@SP",
		"M=M+1"
	);
}

void CodeWriter::writePopTemp(int index) {
	writeAsm(
		"@" + to_string(TEMP + index),
		"D=A", // addr = 5 + i
		"@R13",
		"M=D", // store addr
		"@SP",
		"AM=M-1", // SP--
		"D=M",
		"@R13",
		"A=M",
		"M=D" // *addr=*sp
	);
}

void CodeWriter::writePopSegment(Segment segment, int index) {
	writeAsm(
		"@" + to_string(index),
		"D=A",
		"@" + segmentName(segment),
		"D=D+M", // addr = LCL + i
		"@R13",
		"M=D", // store addr
		"@SP",
		"AM=M-1", // SP--
		"D=M",
		"@R13",
		"A=M",
		"M=D" // *addr=*sp
	);
}

void CodeWriter::writePushPointer(int index) {
	string segment;
	switch (index) {
	case 0: segment = "THIS"; break;
	case 1: segment = "THAT"; break;
	}
	writeAsm(
		"@" + segment, 
		"D=M",
		"@SP",
		"A=M",
		"M=D",
		"@SP",
		"M=M+1"
	);
}

void CodeWriter::writePopPointer(int index) {
	string segment;
	switch (index) {
	case 0: segment = "THIS"; break;
	case 1: segment = "THAT"; break;
	}
	writeAsm(
		"@SP",
		"AM=M-1",
		"D=M",
		"@" + segment,
		"M=D"
	);
}

void CodeWriter::writePushStatic(int index) {
	writeAsm(
		"@" + this->static_name + "." + to_string(index),
		"D=M",
		"D=M",
		"@SP",
		"A=M",
		"M=D",
		"@SP",
		"M=M+1"
	);
}

void CodeWriter::writePopStatic(int index) {
	writeAsm(
		"@SP",
		"AM=M-1",
		"D=M",
		"@" + this->static_name + "." + to_string(index),
		"M=D"
	);
}


string CodeWriter::segmentName(Segment s) {
	switch (s) {
	case Segment::Local:    return "LCL";
	case Segment::Argument: return "ARG";
	case Segment::This:     return "THIS";
	case Segment::That:     return "THAT";
	}
	return "";
}

void CodeWriter::writeAsm(string line) {
	output_file << line << endl;
}

template<typename... Args>
void CodeWriter::writeAsm(Args&&... lines) {
	((output_file << lines << endl), ...);
}

void CodeWriter::close() {
	output_file.close();
}
