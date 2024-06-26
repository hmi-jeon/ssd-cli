#pragma once
#include "ICommand.cpp"

class Erase : public ICommand {
public:
	Erase() {
		this->name = "ERASE";
		this->description = "Delete data from a specific LBA to a specific size. (ex. ERASE 3 5)";
	};

	virtual bool execute(vector<string> args) override {
		if (!checkValidArguments(args))
			return false;

		_doErase(stoi(args[1]), stoi(args[2]));

		return true;
	}

protected:
	void _doErase(int startLBA, int size) {
		while (size > 10) {
			string command = APP_NAME + " " + "E" + " " + to_string(startLBA) + " " + "10";
			system(command.c_str());
			startLBA += 10;
			size -= 10;
		}
		string command = APP_NAME + " " + "E" + " " + to_string(startLBA) + " " + to_string(size);
		system(command.c_str());
	}

private:
	virtual bool checkValidArguments(vector<string> args) override {
		if (args.size() != 3) return false;

		if (!_isNumber(args[1]) || !_isNumber(args[2])) {
			return false;
		}

		if (!_isValidLba(args[1])) {
			return false;
		}

		return _isMaxEraseSize(stoi(args[1]), stoi(args[2]));
	}

	bool _isMaxEraseSize(const int lba, const int size) {
		return (lba + size <= MAX_LBA);
	}
};
