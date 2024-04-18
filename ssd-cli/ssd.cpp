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
		: nand_(nand), buffer_() {
		_initBuffer();
	}

	void command(int argc, char* argv[]) {
		vector<string> cmdString(argv + 1, argv + argc);
		Command* command;

		command = _getCommandType(argc, argv);
		if (command == nullptr) {
			_printInvalidCommand();
			return;
		}

		command->execute(cmdString, nand_, buffer_);

		fstream fs;
		fs.open(BUFFER_FILE_NAME, ios_base::out);
		fs.write(to_string(buffer_.cnt).c_str(), 1);
		for (int idx = 0; idx < 100; ++idx) {
			fs.write(to_string(buffer_.dirty[idx]).c_str(), 1);
		}
		for (int idx = 0; idx < 100; ++idx) {
			fs.write(buffer_.data[idx].c_str(), 8);
		}
		fs.close();
	}

	string getResultFileName() const {
		return RESULT_FILE_NAME;
	}

private:
	enum COMMAND {
		eREAD,
		eWRITE,
		eERASE,
		eFLUSH,
	};

	static constexpr char RESULT_FILE_NAME[] = "result.txt";
	static constexpr char BUFFER_FILE_NAME[] = "buffer.txt";

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

	void _initBuffer() {
		fstream fs;
		fs.open(BUFFER_FILE_NAME, ios_base::in);
		if (fs.is_open() == false) {
			fs.open(BUFFER_FILE_NAME, ios_base::out);
			fs.write(to_string(buffer_.cnt).c_str(), 1);
			for (int idx = 0; idx < 100; ++idx) {
				fs.write(to_string(buffer_.dirty[idx]).c_str(), 1);
			}
			for (int idx = 0; idx < 100; ++idx) {
				buffer_.data[idx] = "00000000";
				fs.write(buffer_.data[idx].c_str(), 8);
			}
		}
		else {
			char temp[1 + 100 + 800 + 1] = {};
			fs.read(temp, 1 + 100 + 800);
			string fileData(temp);
			buffer_.cnt = stoi(fileData.substr(0,1));
			int idx = 1;
			for (int cnt = 0; cnt < 100; ++cnt, ++idx) {
				buffer_.dirty[cnt] = stoi(fileData.substr(idx, 1));
			}
			for (int cnt = 0; cnt < 100; ++cnt, idx += 8) {
				buffer_.data[cnt] = fileData.substr(idx, 8);
			}
		}
		fs.close();
	}

	INAND* nand_;
	WriteBuffer buffer_;
};