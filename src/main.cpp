#include <exception>
#include <fstream>

#include "engine/subspace.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    fstream loggerFile {"log.txt", loggerFile.out};

    subspace::Logger logger {loggerFile, subspace::Logger::Level::Verbose};
    subspace::Config config {"rc/config.json"};

    try {
        logger.logInfo("Launching engine...");

        subspace::initializeVideo();
        logger.logVerbose("Video subsystem initialized");
        
        subspace::Window window("Subspace Engine", config);
        logger.logVerbose("Created window");

        subspace::Renderer renderer(window);
        logger.logVerbose("Created Vulkan instance");
    } catch (const exception& e) {
        logger.logError(e.what());
    }

    return 0;
}
