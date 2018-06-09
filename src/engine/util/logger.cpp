#include "logger.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>

using namespace std;

// This is a bit weird, but I couldn't quite get the line lengths to fit down
// below
using un = unsigned;

namespace subspace {
    Logger::Logger(ostream& output, Level raiseLevel) :
        output_(output), raiseLevel_(raiseLevel)
    {};

    void Logger::log(const string& message, Logger::Level level) {
        if (static_cast<un>(level) >= static_cast<un>(raiseLevel_)) {
            time_t currentTime = time(nullptr);

            output_ << 
                levelToString(level) << 
                put_time(localtime(&currentTime), "%F %T ") <<
                message << endl;
        }
    }

    const char* Logger::levelToString(Logger::Level level) {
        switch (level) {
            case Level::VERBOSE:
                return "[VERBOSE] ";
            case Level::DEBUG:
                return "[DEBUG]   ";
            case Level::INFO:
                return "[INFO]    ";
            case Level::WARNING:
                return "[WARNING] ";
            case Level::ERROR:
                return "[ERROR]   ";
        }
    }
}
