#include "inand.h"
#include <string>
#include <fstream>

using namespace std;

class VirtualNAND : public INAND {
public :
	VirtualNAND() {
		_initNand();
	}

	string read(const int lba) {
		char data[LBA_SIZE + 1]{};
		_readFile(NAND_FILE_NAME, data, lba);
		return string(data);
	}

	void write(const int lba, const string value) {
		char data[NAND_SIZE + 1] = {};
		_readFileAll(NAND_FILE_NAME, data);
		_replaceData(&data[lba * LBA_SIZE], value.c_str(), LBA_SIZE);
		_writeFile(NAND_FILE_NAME, data, NAND_SIZE);
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
	static constexpr int LBA_SIZE = 8;
	static constexpr int MAX_LBA = 100;
	static constexpr int NAND_SIZE = LBA_SIZE * MAX_LBA;
	static constexpr char NAND_FILE_NAME[12] = "nand.txt";
	static constexpr char FORMAT_DATA[LBA_SIZE + 1] = "00000000";

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

	void _readFile(const string fileName, char data[], int lba) {
		fs_.open(fileName.c_str(), ios_base::in);
		fs_.seekg(lba * LBA_SIZE, ios_base::beg);
		fs_.read(data, LBA_SIZE);
		fs_.close();
	}

	void _readFileAll(const string fileName, char data[]) {
		fs_.open(fileName.c_str(), ios_base::in);
		fs_.read(data, NAND_SIZE);
		fs_.close();
	}

	void _writeFile(const string fileName, const char data[], int size) {
		fs_.open(fileName.c_str(), ios_base::out);
		fs_.write(data, size);
		fs_.close();
	}

	void _replaceData(char dst[], const char src[], int size) {
		for (int idx = 0; idx < size; ++idx) {
			dst[idx] = src[idx];
		}
	}
	
	fstream fs_;
};