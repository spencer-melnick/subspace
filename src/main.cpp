#include <iostream>
#include <exception>
#include <thread>
#include <fstream>

#include <fmt/format.h>
#include <vulkan/vulkan.hpp>

#include "engine/subspace.hpp"
#include "generated/default_config.hpp"

using namespace std;
using namespace std::chrono_literals;

int main(int argc, char* argv[]) {
    fstream loggerFile {"log.txt", loggerFile.out};

    subspace::Logger logger {loggerFile, subspace::Logger::Level::VERBOSE};
    subspace::Config config {"rc/config.json"};

    try {
        logger.logInfo("Launching engine...");

        subspace::initializeVideo();
        logger.logVerbose("Video subsystem initialized");
        
        subspace::Window("Subspace Engine", config);
        logger.logVerbose("Created window");
    } catch (const exception& e) {
        logger.logError(e.what());
    }

    return 0;
}
