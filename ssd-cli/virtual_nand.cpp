#include "inand.h"
#include <string>
#include <fstream>

using namespace std;

class VirtualNAND : public INAND{
public :
	VirtualNAND() {
		_initNand();
	}

	void read(const int lba) {
		char buffer[LBA_SIZE + 3] = {'0', 'x'};
		_readFile(NAND_FILE_NAME, &buffer[2], lba);
		_writeFile(RESULT_FILE_NAME, buffer, LBA_SIZE + 2);
	}

	void write(const int lba, const string value) {
		char buffer[NAND_SIZE + 1] = {};
		_readFileAll(NAND_FILE_NAME, buffer);
		_replaceData(&buffer[lba * LBA_SIZE], value.c_str(), LBA_SIZE);
		_writeFile(NAND_FILE_NAME, buffer, NAND_SIZE);
	}

private:
	void _initNand() {
		FILE* file;
		fopen_s(&file, NAND_FILE_NAME, "r");
		if (file == nullptr) {
			fopen_s(&file, NAND_FILE_NAME, "w");
			for (int cnt = 0; cnt < MAX_LBA; ++cnt) {
				fwrite(FORMAT_DATA, 1, LBA_SIZE, file);
			}
		}
		fclose(file);
	}

	void _readFile(const string fileName, char buffer[], int lba) {
		fstream fs;
		fs.open(fileName.c_str(), ios_base::in);
		fs.seekg(lba * LBA_SIZE, ios_base::beg);
		fs.read(buffer, LBA_SIZE);
		fs.close();
	}

	void _readFileAll(const string fileName, char buffer[]) {
		fstream fs;
		fs.open(fileName.c_str(), ios_base::in);
		fs.read(buffer, NAND_SIZE);
		fs.close();
	}

	void _writeFile(const string fileName, const char buffer[], int size) {
		fstream fs;
		fs.open(fileName.c_str(), ios_base::out);
		fs.write(buffer, size);
		fs.close();
	}

	void _replaceData(char dst[], const char src[], int size) {
		for (int idx = 0; idx < size; ++idx) {
			dst[idx] = src[idx];
		}
	}
};