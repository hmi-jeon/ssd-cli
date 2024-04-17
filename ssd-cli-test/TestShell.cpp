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
		string fileName = "ssd-cli.exe";
		string command = fileName + " " + "R" + " " + to_string(lba);
		system(command.c_str());

	  // result.txt open
		ifstream resultFile;
		string data = "";
		resultFile.open("result.txt");
		if (resultFile.is_open()) {
			resultFile >> data;
		}
		return data;
	};

  void write(int lba, string data) override {
		string fileName = "ssd-cli.exe";
		string command = fileName + " " + "W" + " " + to_string(lba) + " " + data;
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

	void checkExistcommand(string command) {
		vector<string> commandList = { "write", "read", "exit" , "help", "fullread", "fullwrite" };

		if (find(commandList.begin(), commandList.end(), command) == commandList.end()) {
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
		checkExistcommand(args[0]);
	}

	void executeCommand() {

		string command = args[0];
		
		int lba;
		if (args.size() >= 2) lba = stoi(args[1]);

		string data;
		if (args.size() == 3) data = args[2];

		if (command == "write") {
			ssdAPI->write(lba, data);
		}

		if (command == "read") {
			ssdAPI->read(lba);
		}

		if (command == "exit") {
			exit();
		}

		if (command == "help") {
			help();
		}

		if (command == "fullread") {
			fullread();
		}

		if (command == "fullwrite") {
			fullwrite(data);
		}
	}

	void inputCommand(string userInput) {
		args = parsingInput(userInput);
		checkInputValidation();
		executeCommand();
	}

	string read(int lba) {
		return ssdAPI->read(lba);;
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

	vector<string> fullread() {
		vector<string> res;
		for (int lba = 0; lba < MAX_SIZE; lba++) {
			res.push_back(read(lba));
		}
		return res;
	};

	void fullwrite(string data) {
		for (int lba = 0; lba < 100; lba++) {
			write(lba, data);
		}
	}

	bool getStatus() {
		return status;
	}

  bool testApp1() {
		vector<string> res;
		string testData = "0x12345678";
		fullwrite(testData);
		res = fullread();

		for (string data : res) {
			if (data != testData)
				return false;
		}
		return true;
	}

	bool testApp2() {
		string oldTestData = "0xAAAABBBB";
		string newTestData = "0x12345678";
		for (int i = 0; i < 30; i++) {
			for (int lba = 0; lba <= 5; lba++) {
				write(lba, oldTestData);
			}
		}
		for (int lba = 0; lba <= 5; lba++) {
			write(lba, newTestData);
		}

		for (int lba = 0; lba <= 5; lba++) {
			if (read(lba) != newTestData)
				return false;
		}
		return true;
	}
  
private:
	bool status = false;
	vector<string> args;
  
};

