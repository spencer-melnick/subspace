#include "logger.hpp"

using namespace std;

namespace subspace {
    Logger::Logger(Level raiseLevel) :
        output_(nullptr), raiseLevel_(raiseLevel)
    {}

    Logger::Logger(ostream& output, Level raiseLevel) :
        output_(&output), raiseLevel_(raiseLevel)
    {}

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

        return "";
    }

    Logger logger(Logger::Level::Verbose);
}
