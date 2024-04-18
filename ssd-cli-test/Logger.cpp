#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cstdio>

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

	void setLoggerMode(LoggerMode mode) {
		this->mode = mode;
	}

	void log(string functionName, string logMsg) {
		string logBuffer;
		if (makeLog(logBuffer, functionName, logMsg) == false) {
			return;
		}
		saveLog(logBuffer);
		if (mode != RUNNER_MODE)
			cout << logBuffer << endl;
	}


private:
	Logger() {

	}
	LoggerMode mode = SHELL_MODE;
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;
	fstream log_fs;

	static constexpr char LATEST_LOG_FILE_NAME[11] = "latest.log";
	static constexpr char UNTIL_LOG_FILE_NAME[11] = "until.log";
	static constexpr int LIMIT_LOG_SIZE = 200;// 1024 * 10; // 10KB

	string makeDateString(DateType dateType) {
		std::time_t now = std::time(nullptr);
		std::tm* current_time{};
		localtime_s(current_time, &now);

		stringstream ss{};
		switch (dateType)
		{
		case LOG_DATE:
			ss << "[" << std::put_time(current_time, "%y.%m.%d %H:%M:%S") << "]";
			break;
		case FILENAME_DATE:
			ss << std::put_time(current_time, "%y%m%d_%Hh%Mm%Ss");
			break;
		default:
			ss << std::put_time(current_time, "%y.%m.%d %H:%M:%S");
		}
		return ss.str();
	}

	bool makeLog(string& logBuffer, string functionName, string logMsg) {
		logBuffer = makeDateString(LOG_DATE) + functionName + ":" + logMsg;
		return true;
	}


	void saveLog(string logBuffer) {
		checkLogFileSize(logBuffer.size());
		openLogFile(log_fs, LATEST_LOG_FILE_NAME);
		log_fs << logBuffer << endl;
		log_fs.close();
	}

	bool openLogFile(fstream& log_fs, const std::string& filename) {
		log_fs.open(filename, std::ios::out | std::ios::app);
		if (!log_fs.is_open()) {
			return false;
		}
	}

	void checkLogFileSize(int logSize) {
		int latestLogFilesize = checkFileSize(LATEST_LOG_FILE_NAME);
		if (latestLogFilesize == -1) return;
		if (latestLogFilesize + logSize > LIMIT_LOG_SIZE) {
			saveUntilLog();
		}
	}

	int checkFileSize(const std::string& filename) {
		std::ifstream file(filename, std::ios::binary);
		if (!file.is_open()) {
			return -1;
		}
		file.seekg(0, std::ios::end);
		streampos size = file.tellg();
		file.close();
		return static_cast<int>(size);
	}

	bool saveUntilLog() {
		makeDateString(FILENAME_DATE);
		string oldFilename = LATEST_LOG_FILE_NAME;
		string newFilename = "until_" + makeDateString(FILENAME_DATE) + ".log";
		if (std::rename(oldFilename.c_str(), newFilename.c_str()) != 0) {
			return false;
		}
		return true;
	}

};