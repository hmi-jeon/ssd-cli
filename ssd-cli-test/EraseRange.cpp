#pragma once
#include "Erase.cpp"

class EraseRange : public Erase {
public:
	EraseRange() {
		this->name = "ERASE_RANGE";
		this->description = "Delete data from startLBA to endLBA (ex. ERASE 20 25)";
	};

	virtual bool execute(vector<string> args) override {
		if (!checkValidArguments(args))
			return false;

		_doErase(stoi(args[1]), stoi(args[2]) - stoi(args[1]));

		return true;
	}
private:
	virtual bool checkValidArguments(vector<string> args) override {
		if (args.size() != 3) return false;

		if (!_isNumber(args[1]) || !_isNumber(args[2])) {
			return false;
		}

		if (!_isValidLba(args[1]) || !_isValidLba(to_string(stoi(args[2])-1))) {
			return false;
		}

		if (!_isValidEraseRangeSize(stoi(args[1]), stoi(args[2]))) {
			return false;
		}

		return true;
	}

	bool _isValidEraseRangeSize(const int startLba, const int endLba) {
		if (endLba <= startLba) return false;
		return (endLba <= MAX_LBA);
	}
};