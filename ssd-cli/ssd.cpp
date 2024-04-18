#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "inand.h"
#include "Command.cpp"

using namespace std;

class SSD {
public:
	SSD(INAND* nand)
		: nand_(nand) {

	}

	void command(int argc, char* argv[]) {
		vector<string> cmdString(argv + 1, argv + argc);
		Command* command;

		command = _getCommandType(argc, argv);
		if (command == nullptr) {
			_printInvalidCommand();
			return;
		}

		command->execute(cmdString, nand_);
	}

	string getResultFileName() const {
		return RESULT_FILE_NAME;
	}

private:
	static constexpr char RESULT_FILE_NAME[] = "result.txt";

	Command* _getCommandType(int argc, char* argv[]) {
		vector<string> cmdString(argv + 1, argv + argc);
		Command* command = nullptr;

		if (argc == 4 && cmdString[0]._Equal("W")) {
			command = new Write();
		}
		else if (argc == 3 && cmdString[0]._Equal("R")) {
			command = new Read();
		}
		else if (argc == 4 && cmdString[0]._Equal("E")) {
			command = new Erase();
		}

		return command;
	}

	void _printInvalidCommand() {
		std::cout << "INVALID COMMAND" << std::endl;
	}

	INAND* nand_;
};