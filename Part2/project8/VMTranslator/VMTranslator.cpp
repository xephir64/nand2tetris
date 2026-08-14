// VMTranslator.cpp : définit le point d'entrée de l'application.
//

#include "VMTranslator.h"

using namespace std;

int process(unique_ptr<CodeWriter>& cw, string filename) {
	ifstream myfile(filename);
	string line;
	stringstream file;
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			file << line << '\n';
		}
	}
	myfile.close();

	unique_ptr<Parser> parser(new Parser(file.str()));

	cout << "Translating " << std::filesystem::path(filename).stem().string() << endl;
	cw->setFileName(filename);

	while (parser->hasMoreCommands()) {
		switch (parser->commandType()) {
		case CommandType::C_ARITHMETIC: cw->writeArithmetic(parser->arg1()); break;
		case CommandType::C_PUSH: cw->writePushPop(parser->commandType(), parser->arg1(), parser->arg2()); break;
		case CommandType::C_POP: cw->writePushPop(parser->commandType(), parser->arg1(), parser->arg2()); break;
		case CommandType::C_LABEL: cw->writeLabel(parser->arg1()); break;
		case CommandType::C_GOTO: cw->writeGoto(parser->arg1()); break;
		case CommandType::C_IF: cw->writeIf(parser->arg1()); break;
		case CommandType::C_FUNCTION: cw->writeFunction(parser->arg1(), parser->arg2()); break;
		case CommandType::C_RETURN: cw->writeReturn(); break;
		case CommandType::C_CALL: cw->writeCall(parser->arg1(), parser->arg2()); break;
		}
		parser->advance();
	}
	return 0;
}

static int process_folder(string folder_name) {
	string sys_file;
	string output_file = folder_name + ".asm";
	vector<string> files;
	for (const auto& entry : fs::directory_iterator(folder_name)) {
		if (entry.is_regular_file()) {
			if (entry.path().extension().compare(".vm") == 0) {
				if (entry.path().filename().compare("Sys.vm") == 0) {
					sys_file = entry.path().string();
				}
				else {
					files.push_back(entry.path().string());
				}
				
			}
		}
	}
	if (sys_file.empty()) {
		cerr << "Sys.vm not found, aborting." << endl;
		return 1;
	}

	unique_ptr<CodeWriter> writer(new CodeWriter(folder_name + "/" + output_file));
	vector<string>::iterator it;
	
	writer->setFileName(sys_file);
	writer->writeInit();
	process(writer, sys_file);

	for (it = files.begin(); it != files.end(); it++) {
		process(writer, *it);
	}
	writer->close();

	return 0;
}

static int process_file(string filename) {
	string output_file = filename.substr(0, filename.find_last_of('.')) + ".asm";
	unique_ptr<CodeWriter> writer(new CodeWriter(output_file));

	process(writer, filename);
	return 0;
}

int main(int argc, char* argv[])
{
	string filename;
	if (argc > 1) {
		filename = argv[1];
	}
	else {
		cerr << "usage: VMTranslator filename.vm" << endl;
		return -1;
	}

	const fs::path path(filename);
	std::error_code ec;
	if (fs::is_directory(path, ec)) {
		return process_folder(filename);
	} else if (fs::is_regular_file(path, ec)) {
		return process_file(filename);
	}

	return 0;
}
