#ifndef CODEWRITER_H
#define CODEWRITER_H

#include <fstream>
#include "Instruction.h"

/* temp segment, from RAM[5] to RAM[12] */
constexpr auto TEMP = 5; 

enum class Segment {
	Local,
	Argument,
	This,
	That
};

enum class Operation {
	Add,
	Sub,
	Or,
	And,
	Neg,
	Not,
	Lt,
	Gt,
	Eq
};

class CodeWriter {
public:
	CodeWriter(string filename);
	void writeArithmetic(string command);
	void writePushPop(CommandType command, string segment, int index);
	void writeLabel(string label);
	void writeGoto(string label);
	void writeIf(string label);
	void writeFunction(string function_name, int num_vars);
	void writeCall(string function_name, int num_args);
	void writeReturn();
	void writeInit();
	void setFileName(string filename);
	void close();
private:
	void writePushSegment(Segment segment, int index);
	void writePushTemp(int index);
	void writePushPointer(int index);
	void writePushStatic(int index);

	void writePopTemp(int index);
	void writePopSegment(Segment segment, int index);
	void writePopPointer(int index);
	void writePopStatic(int index);

	void writeArithmeticMath(Operation op);
	void writeArithmeticComp(Operation op);

	void writeAsm(string line);

	template<typename... Args>
	void writeAsm(Args&&... lines);

	string segmentName(Segment s);
	ofstream output_file;
	string static_name;
	int count_label;
	int call_id;
};

#endif