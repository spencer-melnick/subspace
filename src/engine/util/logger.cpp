#include "logger.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>

using namespace std;

namespace subspace {
    Logger::Logger(Level raiseLevel) :
        output_(nullptr), raiseLevel_(raiseLevel)
    {};

    Logger::Logger(ostream& output, Level raiseLevel) :
        output_(&output), raiseLevel_(raiseLevel)
    {};

    void Logger::log(const string& message, Logger::Level level) {
        unsigned evalLevel = static_cast<unsigned>(level);
        unsigned evalRaiseLevel = static_cast<unsigned>(raiseLevel_);

        if (output_ != nullptr && evalLevel >= evalRaiseLevel) {
            time_t currentTime = time(nullptr);

            *output_ << 
                levelToString(level) << 
                put_time(localtime(&currentTime), "%F %T  ") <<
                message << endl;
        }
    }

    void Logger::setOutput(ostream& output) {
        output_ = &output;
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

    Logger logger(Logger::Level::Verbose);
}
