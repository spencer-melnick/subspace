#pragma once

#include <ostream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>

#include <fmt/format.h>

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
            using string_view = fmt::basic_string_view<char>;

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
             * Creates a logger with no associated output stream. Any logging
             * operations on this logger will fail until an output stream has been
             * specified via Logger::setOutput().
             * 
             * @param raiseLevel The minimum message raise level. Any messages
             * logged with this level or lower will not be written to the
             * output stream.
             */
            Logger(Level raiseLevel = Level::Debug);

            /**
             * Creates a logger that outputs to the specified stream.
             * 
             * @param output The stream to be written to.
             * @param raiseLevel The minimum message raise level. Any messages
             * logged with this level or lower will not be written to the
             * output stream.
             */
            Logger(std::ostream& output, Level raiseLevel = Level::Debug);

            /**
             * Logs a message with the appropriate log level
             */
            template <typename... Args>
            void log(string_view message, Level level, const Args&... args) {
                unsigned evalLevel = static_cast<unsigned>(level);
                unsigned evalRaiseLevel = static_cast<unsigned>(raiseLevel_);

                if (output_ != nullptr && evalLevel >= evalRaiseLevel) {
                    std::time_t currentTime = std::time(nullptr);

                    *output_ << 
                        levelToString(level) << 
                        std::put_time(std::localtime(&currentTime), "%F %T  ") <<
                        fmt::format(message, args...) << std::endl;
                }
            }

            /**
             * Sets the associated output stream
             */
            void setOutput(std::ostream& output);

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

            template <typename... Args>
            inline void logVerbose(string_view message, const Args&... args) {
                log(message, Level::Verbose, args...);
            }

            template <typename... Args>
            inline void logDebug(string_view message, const Args&... args) {
                log(message, Level::Debug, args...);
            }

            template <typename... Args>
            inline void logInfo(string_view message, const Args&... args) {
                log(message, Level::Info, args...);
            }

            template <typename... Args>
            inline void logWarning(string_view message, const Args&... args) {
                log(message, Level::Warning, args...);
            }

            template <typename... Args>
            inline void logError(string_view message, const Args&... args) {
                log(message, Level::Error, args...);
            }

        private:
            std::ostream* output_;
            Level raiseLevel_;
    };

    extern Logger logger;
}
