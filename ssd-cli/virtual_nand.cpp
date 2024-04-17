#include "inand.h"
#include <string>
#include <fstream>

using namespace std;

class VirtualNAND : public INAND{
public :
	static constexpr int LBA_SIZE = 8;
	static constexpr int MAX_LBA = 100;
	static constexpr int NAND_SIZE = LBA_SIZE * MAX_LBA;
	static constexpr char NAND_FILE_NAME[12] = "nand.txt";
	static constexpr char FORMAT_DATA[LBA_SIZE + 1] = "00000000";

	VirtualNAND() {
		_initNand();
	}

	string read(const int lba) {
		char buffer[LBA_SIZE + 1]{};
		_readFile(NAND_FILE_NAME, buffer, lba);	
		return string(buffer);
	}

	void write(const int lba, const string value) {
		char buffer[NAND_SIZE + 1] = {};
		_readFileAll(NAND_FILE_NAME, buffer);
		_replaceData(&buffer[lba * LBA_SIZE], value.c_str(), LBA_SIZE);
		_writeFile(NAND_FILE_NAME, buffer, NAND_SIZE);
	}

	int getLBASize() const {
		return LBA_SIZE;
	}

	int getMaxLBA() const {
		return MAX_LBA;
	}

	int getNandSize() const {
		return NAND_SIZE;
	}

	string getNandFileName() const {
		return NAND_FILE_NAME;
	}

private:

	void _initNand() {
		fs_.open(NAND_FILE_NAME, ios_base::in);
		if (fs_.is_open() == false) {
			fs_.open(NAND_FILE_NAME, ios_base::out);
			for (int cnt = 0; cnt < MAX_LBA; ++cnt) {
				fs_.write(FORMAT_DATA, LBA_SIZE);
			}
		}
		fs_.close();
	}

	void _readFile(const string fileName, char buffer[], int lba) {
		fs_.open(fileName.c_str(), ios_base::in);
		fs_.seekg(lba * LBA_SIZE, ios_base::beg);
		fs_.read(buffer, LBA_SIZE);
		fs_.close();
	}

	void _readFileAll(const string fileName, char buffer[]) {
		fs_.open(fileName.c_str(), ios_base::in);
		fs_.read(buffer, NAND_SIZE);
		fs_.close();
	}

	void _writeFile(const string fileName, const char buffer[], int size) {
		fs_.open(fileName.c_str(), ios_base::out);
		fs_.write(buffer, size);
		fs_.close();
	}

	void _replaceData(char dst[], const char src[], int size) {
		for (int idx = 0; idx < size; ++idx) {
			dst[idx] = src[idx];
		}
	}
	
	fstream fs_;
};