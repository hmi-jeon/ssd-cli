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
		FILE* file;
		fopen_s(&file, "nand.txt", "r");
		if (file == nullptr) {
			fopen_s(&file, "nand.txt", "w");

			char resetData[9] = "00000000";
			for (int i = 0; i < 100; ++i) {
				fwrite(resetData, 1, 8, file);
			}
		}
		fclose(file);
	}

	void read(int lba) {
		FILE* nandFile, *resultFile;
		fopen_s(&nandFile, "nand.txt", "r");
		fopen_s(&resultFile, "result.txt", "w");

		char readData[100][9];

		for (int i = 0; i < 100; i++)
		{
			fread(readData[i], 1, 8, nandFile);
			readData[i][8] = '\0';
		}

		fwrite(readData[lba], 1, 8, resultFile);

		fclose(nandFile);
		fclose(resultFile);
	}

	void write(int lba, string value) {
		fstream fs;
		fs.open("nand.txt", ios_base::in);
		char str[100 * 8];
		fs.read(str, 100 * 8);
		fs.close();

		for (int i = 0; i < 8; ++i) {
			str[lba * 8 + i] = value.at(i);
		}

		FILE* file;
		fopen_s(&file, "nand.txt", "w");
		
		fwrite(str, 8, 100, file);
		fclose(file);
	}
private:
};