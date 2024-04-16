#include <string>
#include <fstream>

using namespace std;

class lNAND {
public:
	virtual void read(int lba) = 0;
	virtual void write(int lba, string value) = 0;
};

class NAND : public lNAND{
public :
	NAND() {
		_initNand();
	}

	void read(int lba) {
		char buffer[LBA_SIZE + 1] = {};
		_readFile(NAND_FILE_NAME, buffer, lba);
		_writeFile(RESULT_FILE_NAME, buffer, LBA_SIZE);
	}

	void write(int lba, string value) {
		char buffer[NAND_SIZE + 1] = {};
		_readFileAll(NAND_FILE_NAME, buffer);
		_replaceData(&buffer[lba * LBA_SIZE], value.c_str(), LBA_SIZE);
		_writeFile(NAND_FILE_NAME, buffer, NAND_SIZE);
	}

private:
	static constexpr int LBA_SIZE = 8;
	static constexpr int MAX_LBA = 100;
	static constexpr int NAND_SIZE = LBA_SIZE * MAX_LBA;
	const char NAND_FILE_NAME[12] = "nand.txt";
	const char RESULT_FILE_NAME[12] = "result.txt";
	const char FORMAT_DATA[LBA_SIZE + 1] = "00000000";

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