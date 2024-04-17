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
	
	string line{};
	int adr[100] = { 0 , };

	TestShell(ISSD* ssdAPI) {
		this->ssdAPI = ssdAPI;
	}

	void inputCommand(string userInput) {
		stringstream ss(userInput);
		vector<string> args;
		string word;

		while (getline(ss, word, ' ')) {
			args.push_back(word);
		}

		vector<string> commnadList = { "write", "read", "exit" , "help", "fullread", "fullwrite"};

		if (find(commnadList.begin(), commnadList.end(), args[0]) == commnadList.end()) {
			cout << "INVALID COMMAND" << endl;
			throw invalid_argument("");
		}

		if (args[0] == "write") {
			ssdAPI->write(stoi(args[1]), args[2]);
		}

		if (args[0] == "read") {
			ssdAPI->read(stoi(args[1]));
		}

		if (args[0] == "exit") {
			exit();
		}

		if (args[0] == "help") {
			help();
		}

		if (args[0] == "fullread") {
			fullread();
		}

		if (args[0] == "fullwrite") {
			fullwrite(args[2]);
		}

	}

	string read(int lba) {
		ssdAPI->read(lba);
		return "";
	}

	void write(int lba, string data) {
		ssdAPI->write(lba, data);
	}

	void exit() {
		isExit = true;
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

	bool getExit() {
		return isExit;
	}

private:
	bool isExit = false;
};

