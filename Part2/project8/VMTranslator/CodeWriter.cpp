#include "CodeWriter.h"
#include <filesystem>
#include <stdexcept>

using std::string;

CodeWriter::CodeWriter(string filename)
    : static_name(std::filesystem::path(filename).stem().string()),
      count_label(0), call_id(0)
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
		writeArithmeticMath(Operation::Add);
	}
	if (command == "sub") {
		writeArithmeticMath(Operation::Sub);
	}
	if (command == "or") {
		writeArithmeticMath(Operation::Or);
	}
	if (command == "and") {
		writeArithmeticMath(Operation::And);
	}
	if (command == "lt") {
		writeArithmeticComp(Operation::Lt);
	}
	if (command == "gt") {
		writeArithmeticComp(Operation::Gt);
	}
	if (command == "eq") {
		writeArithmeticComp(Operation::Eq);
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

void CodeWriter::writeLabel(string label) {
	output_file << "// LABEL: " << label << endl;
	writeAsm("(" + label + ")");
}

void CodeWriter::writeGoto(string label) {
	output_file << "// GOTO " << label << endl;
	writeAsm(
	"@" + label,
	"0;JMP"
	);
}

void CodeWriter::writeIf(string label) {
	output_file << "// if-goto " << label << endl;
	writeAsm(
		"@SP",
		"AM=M-1",
		"D=M", // pop()
		"@" + label,
		"D;JNE");
}

void CodeWriter::writeFunction(string function_name, int num_vars) {
	writeAsm("(" + function_name + ")");
	for (int i = 0; i < num_vars; i++)
		writeAsm("@SP" ,"A=M", "M=0", "@SP", "M=M+1");
}

void CodeWriter::writeCall(string function_name, int num_args) {
	string label = static_name + "$ret." + to_string(++call_id);
	writeAsm(
		"@" + label,
		"D=A",
		"@SP",
		"A=M",
		"M=D",
		"@SP",
		"M=M+1", // push returnAddress
		"@LCL",
		"D=M",
		"@SP",
		"A=M",
		"M=D",
		"@SP",
		"M=M+1", // push LCL
		"@ARG",
		"D=M",
		"@SP",
		"A=M",
		"M=D",
		"@SP",
		"M=M+1", // push ARG
		"@THIS",
		"D=M",
		"@SP",
		"A=M",
		"M=D",
		"@SP",
		"M=M+1", // push THIS
		"@THAT",
		"D=M",
		"@SP",
		"A=M",
		"M=D",
		"@SP",
		"MD=M+1", // push THAT
		"@5",
		"D=D-A",
		"@"+ to_string(num_args),
		"D=D-A",
		"@ARG",
		"M=D", // ARG = SP-5-nArgs
		"@SP",
		"D=M",
		"@LCL",
		"M=D",
		"@"+ function_name,
		"0;JMP",
		"("+label+")"
	);
}

void CodeWriter::writeReturn() {
	writeAsm(
		"@LCL",
		"D=M",
		"@endFrame",
		"M=D", // endFrame = LCL
		"@5",
		"A=D-A",
		"D=M",
		"@retAddr",
		"M=D",  // retAddr = *(endFrame - 5)
		"@SP",
		"AM=M-1",
		"D=M",
		"@ARG",
		"A=M",
		"M=D", // *ARG=pop()
		"@ARG",
		"D=M",
		"@SP",
		"M=D+1", // SP = ARG + 1
		"@endFrame",
		"AM=M-1", // also decrement endFrame (not in the course)
		"D=M",
		"@THAT",
		"M=D", // THAT = *(endFrame - 1)
		"@endFrame",
		"AM=M-1",
		"D=M",
		"@THIS",
		"M=D", // THIS = *(endFrame - 2)
		"@endFrame",
		"AM=M-1",
		"D=M",
		"@ARG",
		"M=D", // ARG = *(endFrame - 3)
		"@endFrame",
		"AM=M-1",
		"D=M",
		"@LCL",
		"M=D", // LCL = *(endFrame - 4)
		"@retAddr",
		"A=M",
		"0;JMP");
}

void CodeWriter::writeInit() {
	writeAsm("@256", "D=A", "@SP", "M=D");
	writeCall("Sys.init", 0);
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

void CodeWriter::writeArithmeticMath(Operation op) {
	string operation;
	switch (op) {
	case Operation::Add: operation = "D+M"; break;
	case Operation::Sub: operation = "M-D"; break;
	case Operation::Or: operation = "D|M"; break;
	case Operation::And: operation = "D&M"; break;
	}
	
	writeAsm(
		"@SP", "AM=M-1", "D=M", "@SP", "AM=M-1", "M=" + operation, "@SP", "M=M+1"
	);
}

void CodeWriter::writeArithmeticComp(Operation op) {
	string operation;
	switch (op) {
	case Operation::Eq: operation = "EQ"; break;
	case Operation::Lt: operation = "LT"; break;
	case Operation::Gt: operation = "GT"; break;
	}
	writeAsm(
		"@SP",
		"AM=M-1",
		"D=M",
		"@SP",
		"AM=M-1",
		"D=M-D",
		"@"+ operation +"_TRUE_" + to_string(count_label),
		"D;J" + operation,
		"@SP",
		"A=M",
		"M=0",
		"@" + operation + "_END_" + to_string(count_label),
		"0;JMP",

		"(" + operation +"_TRUE_" + to_string(count_label) + ")",
		"@SP",
		"A=M",
		"M=-1",

		"(" + operation + "_END_" + to_string(count_label) + ")",
		"@SP",
		"M=M+1"
	);
	count_label++;
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

void CodeWriter::setFileName(string filename) {
	static_name = std::filesystem::path(filename).stem().string();
}

void CodeWriter::close() {
	output_file.close();
}
