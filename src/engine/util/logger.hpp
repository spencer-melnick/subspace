#pragma once

#include <ostream>
#include <string>

/**
 * @file
 * @author Spencer Melnick <smelnick97@gmail.com>
 * 
 * Defines a simple logger interface with different log levels, date and time
 * formatting, and file output.
 */

namespace subspace {
    /**
     * Class that provides a simple logger interface.
     */
    class Logger {
        public:
            /**
             * The different logger message levels.
             */
            enum class Level : unsigned {
                Verbose = 0,
                Debug = 1,
                Info = 2,
                Warning = 3,
                Error = 4
            };

            /**
             * Creates a logger that outputs to the specified stream.
             * 
             * @param output The stream to be written to.
             * @param raiseLevel The minimum message raise level. Any messages
             * logged with this level or lower will not be written to the
             * output stream.
             */
            Logger(std::ostream& output, Level raiseLevel = Level::Debug);
            void log(const std::string& message, Level level);

            inline void logVerbose(const std::string& message) {
                log(message, Level::Verbose);
            }

            inline void logDebug(const std::string& message) {
                log(message, Level::Debug);
            }

            inline void logInfo(const std::string& message) {
                log(message, Level::Info);
            }

            inline void logWarning(const std::string& message) {
                log(message, Level::Warning);
            }

            inline void logError(const std::string& message) {
                log(message, Level::Error);
            }

            /**
             * Converts a log level to its string equivalent for output. Each
             * string is formatted with all capital letters and surrounding 
             * braces. These string equivalents also contain whitespace such 
             * that each string is the same length.
             * 
             * @param level The log level to be converted.
             * 
             * @return The string equivalent as a string literal.
             */
            static const char* levelToString(Level level);

        private:
            Level raiseLevel_;
            std::ostream& output_;
    };
}
