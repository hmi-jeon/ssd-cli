#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "inand.h"
#include "ICommand.h"

using namespace std;

class SSD {
public:
	SSD(INAND* nand)
		: nand_(nand), buffer_() {
	}

	void init(vector<ICommand*>& commandVectors) {
		LBA_SIZE = nand_->getLBASize();
		MAX_LBA = nand_->getMaxLBA();
		BUFFER_SIZE = 1 + MAX_LBA + LBA_SIZE * MAX_LBA;
	
		_initBuffer();
		ICommand::setBuffer(buffer_);
		
		commandVectors_ = commandVectors;
	}

	void command(int argc, char* argv[]) {
		ICommand* command = _getCommandType(argc, argv);
		if (command == nullptr) {
			_printInvalidCommand();
			return;
		}

		vector<string> cmdString(argv + 1, argv + argc);
		command->execute(cmdString, nand_);

		_openBufferFile(ios_base::out);
		_writeToBufferFile();
		_closeBufferFile();
	}

	string getResultFileName() const {
		return RESULT_FILE_NAME;
	}

private:
	static constexpr char RESULT_FILE_NAME[] = "result.txt";
	static constexpr char BUFFER_FILE_NAME[] = "buffer.txt";

	ICommand* _getCommandType(int argc, char* argv[]) {
		if (argc < 2) return nullptr;

		for (auto& cmd : commandVectors_) {
			if (cmd->getCommandCode() == string(argv[1])) {
				return cmd;
			}
		}

		return nullptr;
	}

	void _printInvalidCommand() {
		std::cout << "INVALID COMMAND" << std::endl;
	}

	void _initBuffer() {
		_openBufferFile(ios_base::in);
		if (_isBufferFileOpened() == false) {
			_openBufferFile(ios_base::out);
			_resetWriteBuffer();
			_writeToBufferFile();
		}
		else {
			_readDataFromBufferFile();
		}
		_closeBufferFile();
	}

	void _openBufferFile(ios_base::openmode mode) {
		fs_.open(BUFFER_FILE_NAME, mode);
	}

	bool _isBufferFileOpened() {
		return fs_.is_open();
	}

	void _closeBufferFile() {
		fs_.close();
	}

	void _writeBufferCount() {
		fs_.write(to_string(buffer_.cnt).c_str(), 1);
	}

	void _writeBufferDirtyBit() {
		for (int idx = 0; idx < MAX_LBA; ++idx) {
			fs_.write(to_string(buffer_.dirty[idx]).c_str(), 1);
		}
	}

	void _writeBufferData() {
		for (int idx = 0; idx < MAX_LBA; ++idx) {
			fs_.write(buffer_.data[idx].c_str(), LBA_SIZE);
		}
	}

	void _writeToBufferFile() {
		_writeBufferCount();
		_writeBufferDirtyBit();
		_writeBufferData();
	}

	void _resetWriteBuffer() {
		buffer_.cnt = 0;
		for (int idx = 0; idx < MAX_LBA; ++idx) {
			buffer_.dirty[idx] = 0;
			buffer_.data[idx] = "00000000";
		}
	}

	void _readDataFromBufferFile() {
		char* temp = (char*)malloc(BUFFER_SIZE + 1);
		fs_.read(temp, BUFFER_SIZE);
		temp[BUFFER_SIZE] = '\0';
		string fileData(temp);
		buffer_.cnt = stoi(fileData.substr(0, 1));
		int idx = 1;
		for (int cnt = 0; cnt < MAX_LBA; ++cnt, ++idx) {
			buffer_.dirty[cnt] = stoi(fileData.substr(idx, 1));
		}
		for (int cnt = 0; cnt < MAX_LBA; ++cnt, idx += LBA_SIZE) {
			buffer_.data[cnt] = fileData.substr(idx, LBA_SIZE);
		}
	}

	INAND* nand_;
	WriteBuffer buffer_;
	fstream fs_;
	vector<ICommand*> commandVectors_;

	int LBA_SIZE = 8;
	int MAX_LBA = 100;
	int BUFFER_SIZE;
};