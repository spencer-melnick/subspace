#include "config.hpp"

#include <fstream>

#include "generated/default_config.hpp"

using namespace std;
using json = nlohmann::json;

namespace subspace {
    Config::Config(const string& filename)
        : filename_ {filename}
    {
        config_ = json::parse(DEFAULT_CONFIG_STRING);
        fstream file {filename_};

        if (file.is_open()) {
            json new_config;
            
            try {
                file >> new_config;
                config_.merge_patch(new_config);
            } catch (json::parse_error& e) {
                // Swallow parse errors - we still have a good config from our
                // default
            }

            file.close();
        }

        fullscreen_ = config_["fullscreen"];

        xResolution_ = config_["x_resolution"];
        yResolution_ = config_["y_resolution"];
    }

    Config::~Config() {
        fstream file {filename_, file.out};

        if (!file.is_open()) {
            return;
        }

        // Write config.json with tabs
        file << setfill('\t') << setw(1) << config_ << endl;
        file.close();
    }

    void Config::setFullscreen(bool fullscreen) {
        if (fullscreen != fullscreen_) {
            fullscreen_ = fullscreen;
            config_["fullscreen"] = fullscreen_;
        }
    }

    bool Config::isFullscreen() const {
        return fullscreen_;
    }


    void Config::setXResolution(unsigned int x) {
        if (xResolution_ != x) {
            xResolution_ = x;
            config_["x_resolution"] = xResolution_;
        }
    }

    unsigned int Config::getXResolution() const {
        return xResolution_;
    }

    void Config::setYResolution(unsigned int y) {
        if (yResolution_ != y) {
            yResolution_ = y;
            config_["y_resolution"] = yResolution_;
        }
    }

    unsigned int Config::getYResolution() const {
        return yResolution_;
    }
}
