#include "config.hpp"

#include <fstream>

#include <nlohmann/json.hpp>

using namespace subspace;
using json = nlohmann::json;

Config::Config(const string& filename) {
    fstream file {filename};
    json j;

    if (file.is_open()) {
        file >> j;
    }

    try {
        fullscreen_ = j.at("fullscreen");
    } catch (json::out_of_range& e) {
        fullscreen_ = false;
    }
}

bool Config::isFullscreen() const {
    return fullscreen_;
}
