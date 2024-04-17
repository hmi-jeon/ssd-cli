#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <process.h>
#include <sstream>
#include <vector>
#include <stdexcept>

#define interface struct
#define MAX_SIZE 100

using namespace std;

interface ISSD {
	virtual string read(int lba) = 0;
	virtual void write(int lba, string data) = 0;
};

class ssdAPI : public ISSD {
public:
	string read(int lba) override {
		// ssd.exe call
		//system(ssd.exe read lba);

		// result.txt open
		ifstream resultFile;
		string data = "";
		resultFile.open("result.txt");
		if (resultFile.is_open()) {
			resultFile >> data;
			cout << data << endl;
		}
		return data;
	};

	void write(int lba, string data) override {
		string command = "ssd.exe W " + to_string(lba) + " " + data;
		system(command.c_str());
	};
};

class TestShell {
public:
	ISSD* ssdAPI;
	int adr[100] = { 0 , };

	TestShell(ISSD* ssdAPI) {
		this->ssdAPI = ssdAPI;
	}

	vector<string> parsingInput(string inputString) {
		stringstream ss(inputString);
		vector<string> argList;
		string arg;

		while (getline(ss, arg, ' ')) {
			argList.push_back(arg);
		}

		return argList;
	}

	void checkExistCommnad(string command) {
		vector<string> commnadList = { "write", "read", "exit" , "help", "fullread", "fullwrite" };

		if (find(commnadList.begin(), commnadList.end(), command) == commnadList.end()) {
			cout << "INVALID COMMAND" << endl;
			throw invalid_argument("");
		}
	}

	void checkNumberOfArguments(vector<string> args) {
		if (args.size() < 1) throw invalid_argument("");
		
		string command = args[0];

		if (command == "exit" || command == "help" || command == "fullread") {
			if (args.size() != 1) throw invalid_argument("");
		}

		if (command == "read" || command == "fullwrite") {
			if (args.size() != 2) throw invalid_argument("");
		}

		if (command == "write") {
			if (args.size() != 3) throw invalid_argument("");
		}
	}

	void checkInputValidation() {
		checkNumberOfArguments(args);
		checkExistCommnad(args[0]);
	}

	void executeCommand() {

		string commnad = args[0];
		
		int lba;
		if (args.size() >= 2) lba = stoi(args[1]);

		string data;
		if (args.size() == 3) data = args[2];

		if (commnad == "write") {
			ssdAPI->write(lba, data);
		}

		if (commnad == "read") {
			ssdAPI->read(lba);
		}

		if (commnad == "exit") {
			exit();
		}

		if (commnad == "help") {
			help();
		}

		if (commnad == "fullread") {
			fullread();
		}

		if (commnad == "fullwrite") {
			fullwrite(data);
		}
	}

	void inputCommand(string userInput) {
		args = parsingInput(userInput);
		checkInputValidation();
		executeCommand();
	}

	string read(int lba) {
		ssdAPI->read(lba);
		return "";
	}

	void write(int lba, string data) {
		ssdAPI->write(lba, data);
	}

	void exit() {
		status = true;
	};

	void help() {
		cout << "--help--" << endl;
		cout << "--[write]--" << endl;
		cout << "--format: write [LBA] [DATA]--" << endl;
	};

	void fullread() {
		for (int lba = 0; lba < MAX_SIZE; lba++) {
			ssdAPI->read(lba);
		}
	};

	void fullwrite(string data) {
		for (int lba = 0; lba < 100; lba++) {
			ssdAPI->write(lba, data);
		}
	}

	bool getStatus() {
		return status;
	}

private:
	bool status = false;
	vector<string> args;
};

