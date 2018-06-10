#include "logger.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>

using namespace std;

namespace subspace {
    Logger::Logger(ostream& output, Level raiseLevel) :
        output_(output), raiseLevel_(raiseLevel)
    {};

    void Logger::log(const string& message, Logger::Level level) {
        if (static_cast<unsigned>(level) >= static_cast<unsigned>(raiseLevel_)) {
            time_t currentTime = time(nullptr);

            output_ << 
                levelToString(level) << 
                put_time(localtime(&currentTime), "%F %T ") <<
                message << endl;
        }
    }

    const char* Logger::levelToString(Logger::Level level) {
        switch (level) {
            case Level::Verbose:
                return "[VERBOSE] ";
            case Level::Debug:
                return "[DEBUG]   ";
            case Level::Info:
                return "[INFO]    ";
            case Level::Warning:
                return "[WARNING] ";
            case Level::Error:
                return "[ERROR]   ";
        }
    }
}
