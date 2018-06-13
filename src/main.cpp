#include <exception>
#include <fstream>

#include "engine/subspace.hpp"

using namespace std;
using namespace subspace;

int main() {
    fstream logFile("log.txt", ios::out | ios::trunc);
    logger.setOutput(logFile);

    try {
        logger.logInfo("Starting Subspace Engine...");
        initializeVideo();

        Config config {"rc/config.json"};
        Window window("Subspace Engine", config);

        RenderContext context(window);
    } catch (const exception& e) {
        logger.logError(e.what());
    }
    terminateVideo();

    return 0;
}
