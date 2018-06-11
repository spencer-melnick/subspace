#include <exception>
#include <fstream>

#include "engine/subspace.hpp"

using namespace std;
using namespace subspace;

int main(int argc, char* argv[]) {
    fstream logFile("log.txt", ios::out | ios::trunc);
    logger.setOutput(logFile);

    try {
        logger.logInfo("Starting Subspace Engine...");
        initializeVideo();

        Config config {"rc/config.json"};
        Window window("Subspace Engine", config);

        RenderContext context(window);

        terminateVideo();
    } catch (const exception& e) {
        subspace::logger.logError(e.what());
    }

    return 0;
}
