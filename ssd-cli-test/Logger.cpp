#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#define  print(str) log(__FUNCTION__,str)

using namespace std;

enum LoggerMode {
	SHELL_MODE,
	RUNNER_MODE
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
		saveLogger(logBuffer);
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
	static constexpr int LIMIT_LOG_SIZE = 200;// 1024 * 10; // 10KB


	
	streampos checkFileSize(const std::string& filename) {
		std::ifstream file(filename, std::ios::binary);

		if (!file.is_open()) {
			return -1; // 에러 시 -1 반환
		}

		file.seekg(0, std::ios::end);
		std::streampos size = file.tellg();

		file.close();

		return size;
	}

	bool openLogFile(fstream& log_fs, const std::string& filename) {
		log_fs.open(filename, std::ios::out | std::ios::app);
		if (!log_fs.is_open()) {
			return false;
		}
	}

	void saveUntilLog()	{		
		cout << "saveUntilLog : file size over" << endl;
	}

	void saveLogger(string logBuffer) {

		streampos latestLogFilesize = checkFileSize(LATEST_LOG_FILE_NAME);
		if(latestLogFilesize == -1) return;
		if (static_cast<int>(latestLogFilesize) + logBuffer.size() > LIMIT_LOG_SIZE){
			saveUntilLog();
		}

		openLogFile(log_fs, LATEST_LOG_FILE_NAME);
		log_fs << logBuffer << endl;
		log_fs.close();
	}

	string makeDateString() {
		std::time_t now = std::time(nullptr);
		std::tm* current_time = std::localtime(&now);
		stringstream ss{};
		ss << "[" << std::put_time(current_time, "%y.%m.%d %H:%M:%S") << "]";
		return ss.str();
	}

	bool makeLog(string& logBuffer, string functionName, string logMsg) {
		logBuffer = makeDateString() + functionName + ":" + logMsg;
		return true;
	}
};