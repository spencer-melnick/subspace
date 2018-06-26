#include <exception>
#include <fstream>
#include <thread>

#include "engine/subspace.hpp"
#include "engine/video/vulkan/vulkan_renderer.hpp"

using namespace std;
using namespace subspace;

int main() {
    fstream logFile("log.txt", ios::out | ios::trunc);
    logger.setOutput(logFile);

    try {
        logger.logInfo("Starting Subspace Engine...");
        Config config {"rc/config.json"};

        VulkanRenderer renderer;
    } catch (const exception& e) {
        logger.logError(e.what());
    }

    return 0;
}
