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

class CodeWriter {
public:
	CodeWriter(string filename);
	void writeArithmetic(string command);
	void writePushPop(CommandType command, string segment, int index);
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

	void writeAsm(string line);

	template<typename... Args>
	void writeAsm(Args&&... lines);

	string segmentName(Segment s);
	ofstream output_file;
	string static_name;
	int count_label;
};

#endif