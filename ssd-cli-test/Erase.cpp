#pragma once
#include "ICommand.cpp"
#include <vector>

class Erase : public ICommand {
public:
	Erase() {
		this->name = "ERASE";
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

		if (!_isValidLba(args[1]) || !_isValidEraseSize(args[2])) {
			return false;
		}

		return _isMaxEraseSize(stoi(args[1]), stoi(args[2]));
	}

	bool _isValidEraseSize(const string size) {
		for (const char& c : size) {
			if (!std::isdigit(c)) {
				return false;
			}
		}

		return true;
	}

	bool _isMaxEraseSize(const int lba, const int size) {
		return (lba + size <= MAX_LBA);
	}
};
