// VMTranslator.cpp : définit le point d'entrée de l'application.
//

#include "VMTranslator.h"

using namespace std;

int main(int argc, char* argv[])
{
	string filename;
	if (argc > 1) {
		filename = argv[1];
	}
	else {
		cerr << "usage: VMTranslator filename.vm" << endl;
		return 1;
	}
	ifstream myfile(filename);
	string line;
	stringstream file;
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			file << line << '\n';
		}
	}
	myfile.close();
	string output_file = filename.substr(0, filename.find_last_of('.')) + ".asm";

	unique_ptr<Parser> parser(new Parser(file.str()));
	unique_ptr<CodeWriter> writer(new CodeWriter(output_file));

	while (parser->hasMoreCommands()) {
		switch (parser->commandType()) {
			case CommandType::C_ARITHMETIC: writer->writeArithmetic(parser->arg1()); break;
			case CommandType::C_PUSH: writer->writePushPop(parser->commandType(), parser->arg1(), parser->arg2()); break;
			case CommandType::C_POP: writer->writePushPop(parser->commandType(), parser->arg1(), parser->arg2()); break;
		}
		parser->advance();
	}
	writer->close();

	return 0;
}
