#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "inand.h"

using namespace std;

struct WriteBuffer {
	string data[100];
	int dirty[100];
	int cnt;
};

class Command {
public:
	virtual void execute(vector<string> cmdString, INAND* nand_, WriteBuffer& buffer) = 0;

protected:
	bool _isValidLba(const int lba) {
		return (lba >= 0 && lba < 100);
	}

	bool _isValidValue(const string value) {
		if (value.size() != 10)
			return false;

		if (value.substr(0, 2) != "0x")
			return false;

		for (const char& c : value.substr(2)) {
			if (!isxdigit(c)) {
				return false;
			}
		}
		return true;
	}

	bool isNumber(const string& str) {
		for (char const& c : str) {
			if (std::isdigit(c) == 0) return false;
		}
		return true;
	}

	void _printInvalidCommand() {
		std::cout << "INVALID COMMAND" << std::endl;
	}
};

class Read : public Command {
public:
	virtual void execute(vector<string> cmdString, INAND* nand, WriteBuffer& buffer) override {
		if (cmdString.size() != cmdSize) {
			_printInvalidCommand();
			return;
		}

		if (!isNumber(cmdString[1])) {
			_printInvalidCommand();
			return;
		}

		if (!_isValidLba(stoi(cmdString[1]))) {
			_printInvalidCommand();
			return;
		}

		int lba = stoi(cmdString[1]);
		string readData;
		if (buffer.dirty[lba] == 0) {
			readData = nand->read(lba);
		}
		else {
			readData = buffer.data[lba];
		}
		_writeResult(readData);
	}

private:
	static constexpr char RESULT_FILE_NAME[] = "result.txt";
	static constexpr int cmdSize = 2;

	void _writeResult(string readData) {
		fstream fs;
		fs.open(RESULT_FILE_NAME, ios_base::out);
		fs.write(("0x" + readData).c_str(), readData.size() + 2);
		fs.close();
	}
};

class Write : public Command {
public:
	virtual void execute(vector<string> cmdString, INAND* nand, WriteBuffer& buffer) override {
		if (cmdString.size() != cmdSize) {
			_printInvalidCommand();
			return;
		}

		if (!isNumber(cmdString[1])) {
			_printInvalidCommand();
			return;
		}

		if (!_isValidLba(stoi(cmdString[1])) || !_isValidValue(cmdString[2])) {
			_printInvalidCommand();
			return;
		}

		int lba = stoi(cmdString[1]);
		string writeData = cmdString[2].substr(2);
		
		buffer.data[lba] = writeData;
		buffer.dirty[lba] = 1;
		buffer.cnt++;

		if (buffer.cnt >= 10) {
			for (int idx = 0; idx < 100; idx++) {
				if (buffer.dirty[idx] == 1) {
					nand->write(idx, buffer.data[idx]);
				}
				buffer.dirty[idx] = 0;
			}
			buffer.cnt = 0;
		}
	}
	
private:
	static constexpr int cmdSize = 3;
};

class Erase : public Command {
public:
	virtual void execute(vector<string> cmdString, INAND* nand, WriteBuffer& buffer) override {
		if (cmdString.size() != cmdSize) {
			_printInvalidCommand();
			return;
		}

		if (!isNumber(cmdString[1]) || !isNumber(cmdString[2])) {
			_printInvalidCommand();
			return;
		}

		int lba = stoi(cmdString[1]);
		int size = stoi(cmdString[2]);

		if (!_isValidLba(lba) || !_isValidEraseSize(size)) {
			_printInvalidCommand();
			return;
		}
		
		for (int i = 0; i < size; i++) {
			nand->write(lba + i, "00000000");
		}
	}

private:
	static constexpr int cmdSize = 3;

	bool _isValidEraseSize(const int size) {
		return (size > 0 && size <= 10);
	}
};