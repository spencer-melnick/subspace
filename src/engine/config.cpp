#include "config.hpp"

#include <fstream>

#include "generated/default_config.hpp"

using namespace std;
using json = nlohmann::json;

using namespace subspace;

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
            // Swallow parse errors - we still have a good config from our default
        }

        file.close();
    }

    fullscreen_ = config_["fullscreen"];
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
