#include <utils/logger.hpp>

namespace util {

bool Logger::debug = false;

void Logger::SetDebugActive() {
	debug = true;
}

bool Logger::print(const char* callerFun,
	const std::string& logType, 
	const std::string& message) {
	std::cout << 
		"[" << logType << "]" <<
		" at function " << callerFun << "\n" <<
		"\t" << message << std::endl;
	return true;
}

bool Logger::print(const char* callerFile, const int callerLine,
	const std::string& logType, const std::string& message) {
	std::cout <<
		"[" << logType << "]" <<
		" at file \"" << callerFile << "\", line " << callerLine << ":\n" <<
		"\t" << message << std::endl;
	return true;
}

bool Logger::Log(const std::string& message, const char* callerFun) {
	return print(callerFun, "LOG", message);
}
bool Logger::LogDebug(const std::string& message, const char* callerFun) {
	if (debug) {
		print(callerFun, "DEBUG", message);
	}
	return true;
}
bool Logger::LogWarning(const std::string& message, const char* callerFile,
	const char* callerFun,const int callerLine) {
	return print(callerFile, callerLine, "!WARNING!", message);
}
bool Logger::LogError(const std::string& message, const char* callerFile,
	const char* callerFun, const int callerLine) {
	return print(callerFile, callerLine, "ERROR", message);
}

}