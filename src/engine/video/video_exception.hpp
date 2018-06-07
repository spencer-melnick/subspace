#pragma once

/**
 * @file
 * @author Spencer Melnick <smelnick97@gmail.com>
 * 
 * Class for video subsystem exceptions.
 */

#include <exception>

namespace subspace {
    /**
     * Class for video subsystem exceptions.
     */
    class VideoException: public std::exception {
        public:
            /**
             * Type of a video subsystem exception
             */
            enum class Type {
                NONE,
                SDL2_INIT_FAILURE,
                WINDOW_CREATE_FAILURE
            };

            /**
             * Creates a video subsystem exception.
             * 
             * @param type Video exception type
             */
            VideoException(const Type& type);

            /**
             * Extends the standard exception interface.
             * 
             * @return The exception message as a null terminated string.
             */
            const char* what() const noexcept override;

            /**
             * Get the internal video exception type.
             * 
             * @return The exception type as an enum.
             */
            Type getType() const noexcept;

        private:
            Type type_;
    };
}
