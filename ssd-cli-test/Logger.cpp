#include "Logger.hpp"


void Logger::setLoggerMode(LoggerMode mode){
	this->mode = mode;
}

void Logger::log(string functionName, string logMsg) {
	string logBuffer;
	if (makeLog(logBuffer, functionName, logMsg) == false) {
		return;
	}
	saveLog(logBuffer);
	if (mode != RUNNER_MODE)
		cout << logBuffer << endl;
}


string Logger::makeDateString(DateType dateType) {
	std::time_t now = std::time(nullptr);
	std::tm current_time;
	localtime_s(&current_time, &now);

	stringstream ss{};
	switch (dateType)
	{
	case LOG_DATE:
		ss << "[" << std::put_time(&current_time, "%y.%m.%d %H:%M:%S") << "]";
		break;
	case FILENAME_DATE:
		ss << std::put_time(&current_time, "%y%m%d_%Hh%Mm%Ss");
		break;
	default:
		ss << std::put_time(&current_time, "%y.%m.%d %H:%M:%S");
	}
	return ss.str();
}

bool Logger::makeLog(string& logBuffer, string functionName, string logMsg) {

	logBuffer = makeDateString(LOG_DATE) + padString(functionName) + ":" + logMsg;
	return true;
}

string Logger::padString(const string& input) {
	if (input.length() >= FORMAT_LENGTH) {
		return input;
	}
	return input + std::string(FORMAT_LENGTH - input.length(), ' ');
}

void Logger::saveLog(string logBuffer) {
	checkLogFileSize(logBuffer.size());
	openLogFile(log_fs, LATEST_LOG_FILE_NAME);
	log_fs << logBuffer << endl;
	log_fs.close();
}

bool Logger::openLogFile(fstream& log_fs, const std::string& filename) {
	log_fs.open(filename, std::ios::out | std::ios::app);
	if (!log_fs.is_open()) {
		return false;
	}
}

void Logger::checkLogFileSize(int logSize) {
	int latestLogFilesize = checkFileSize(LATEST_LOG_FILE_NAME);
	if (latestLogFilesize == -1) return;
	if (latestLogFilesize + logSize > LIMIT_LOG_SIZE) {
		saveUntilLog();
		compressLogFile();
	}
}

int Logger::checkFileSize(const std::string& filename) {
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open()) {
		return -1;
	}
	file.seekg(0, std::ios::end);
	streampos size = file.tellg();
	file.close();
	return static_cast<int>(size);
}

bool Logger::saveUntilLog() {
	makeDateString(FILENAME_DATE);
	string oldFilename = LATEST_LOG_FILE_NAME;
	string newFilename = "until_" + makeDateString(FILENAME_DATE) + ".log";
	return renameFile(oldFilename, newFilename);
}

bool Logger::renameFile(string oldName, string neName)
{
	if (std::rename(oldName.c_str(), neName.c_str()) != 0) {
		return false;
	}
	return true;
}

void Logger::compressLogFile()
{
	string strDirName = "";

	vector<string> logFiles;

	findLogFiles(logFiles);

	if(isOverLogFileThreshold(logFiles))
		renameFileExtension(logFiles);
}

bool Logger::isOverLogFileThreshold(vector<string>& logFiles)
{
	return logFiles.size() > LOG_FILE_THRESHOLD;
}

void Logger::renameFileExtension(vector<string>& logFiles)
{
	for (int i = 0; i < logFiles.size() - LOG_FILE_THRESHOLD; i++)
	{
		string oldFilename = logFiles[i];
		size_t pos = logFiles[i].find(".log");
		renameFile(oldFilename, logFiles[i].replace(pos, 4, ".zip"));
	}
}

void Logger::findLogFiles(vector<string>& logFiles)
{
	string logPattern = "until*.log";
	HANDLE hFind;
	WIN32_FIND_DATAA data;
	if ((hFind = FindFirstFileA(logPattern.c_str(), &data)) != INVALID_HANDLE_VALUE)
	{
		do {
			logFiles.emplace_back(string(data.cFileName));
		} while (FindNextFileA(hFind, &data) != 0);
		FindClose(hFind);
	}
}
