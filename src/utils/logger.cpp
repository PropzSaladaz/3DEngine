#include <utils/logger.hpp>

namespace util {

bool Logger::debug = false;

void Logger::SetDebugActive() {
	debug = true;
}

void Logger::print(const char* callerFun, 
	const std::string& logType, 
	const std::string& message) {
	std::cout << 
		"[" << logType << "]" <<
		" at function " << callerFun << "\n" <<
		"\t" << message << std::endl;
}

void Logger::print(const char* callerFile, const int callerLine,
	const std::string& logType, const std::string& message) {
	std::cout <<
		"[" << logType << "]" <<
		" at file \"" << callerFile << "\", line " << callerLine << ":\n" <<
		"\t" << message << std::endl;
}

void Logger::Log(const std::string& message, const char* callerFun) {
	print(callerFun, "LOG", message);
}
void Logger::LogDebug(const std::string& message, const char* callerFun) {
	if (debug) {
		print(callerFun, "DEBUG", message);
	}
}
void Logger::LogWarning(const std::string& message, const char* callerFile, 
	const char* callerFun,const int callerLine) {
	print(callerFile, callerLine, "!WARNING!", message);
}
void Logger::LogError(const std::string& message, const char* callerFile,
	const char* callerFun, const int callerLine) {
	print(callerFile, callerLine, "ERROR", message);
}

}