#include <string>
#include <fstream>

using namespace std;

class NAND {
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

	}
private:
};