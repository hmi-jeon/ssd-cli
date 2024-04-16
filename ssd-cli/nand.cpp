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