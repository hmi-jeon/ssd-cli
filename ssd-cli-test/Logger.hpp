#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cstdio>
#include <windows.h>
#include <vector>

#define  print(str) log(__FUNCTION__,str)

using namespace std;

enum LoggerMode {
	SHELL_MODE,
	RUNNER_MODE
};

enum DateType {
	LOG_DATE,
	FILENAME_DATE
};

class Logger {
public:
	static Logger& getInstance() {
		static Logger instance;
		return instance;
	}

	void setLoggerMode(LoggerMode mode);
	void log(string functionName, string logMsg);

private:
	Logger() {

	}
	LoggerMode mode = SHELL_MODE;
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;
	fstream log_fs;

	static constexpr char LATEST_LOG_FILE_NAME[11] = "latest.log";
	static constexpr int LIMIT_LOG_SIZE = 200;// 1024 * 10; // 10KB
	static constexpr int LOG_FILE_THRESHOLD = 3;
	static constexpr int FORMAT_LENGTH = 40;

	string makeDateString(DateType dateType);
	bool makeLog(string& logBuffer, string functionName, string logMsg);
	string padString(const string& input);
	void saveLog(string logBuffer);
	bool openLogFile(fstream& log_fs, const std::string& filename);
	void checkLogFileSize(int logSize);
	int checkFileSize(const std::string& filename);
	bool saveUntilLog();
	bool renameFile(string oldName, string neName);
	void compressLogFile();
};