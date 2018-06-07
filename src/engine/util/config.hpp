#pragma once

/**
 * @file
 * @author Spencer Melnick <smelnick97@gmail.com>
 * 
 * Allows access of simple program configuration files stored in
 * the JSON format.
 */

#include <string>

#include <nlohmann/json.hpp>

namespace subspace {
    /**
     * Class that allows access of simple program configuration files stored in
     * the JSON format.
     */
    class Config {
        public:
            /**
             * Constructor that loads program configuration from a JSON file
             * and associates the object with that file path. The JSON file is
             * rewritten on deconstruction
             * 
             * @param filename Path to the JSON configuration file
             */
            Config(const std::string& filename);

            /**
             * Destructor that attempts to save the current configuration to the
             * associated JSON file if possible
             */
            ~Config();

            void setFullscreen(bool fullscreen);
            bool isFullscreen() const;

            void setXResolution(unsigned int x);
            unsigned int getXResolution() const;

            void setYResolution(unsigned int y);
            unsigned int getYResolution() const;

        private:
            std::string filename_;
            nlohmann::json config_;

            bool fullscreen_;
            unsigned int xResolution_, yResolution_;
    };
}
