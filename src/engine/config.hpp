#pragma once

#include <string>

#include <nlohmann/json.hpp>

using string = std::string;
using json = nlohmann::json;

namespace subspace {
    class Config {
        public:
            Config(const string& filename);
            ~Config();

            void setFullscreen(bool fullscreen);
            bool isFullscreen() const;

        private:
            bool fullscreen_;
            string filename_;
            json config_;
    };
}
