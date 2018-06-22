#include <exception>
#include <fstream>
#include <thread>

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

        RenderContext context;
        Window window(context, "Subspace Engine", config);

        Shader testShader(context, "rc/shaders/test/vert.spv", "rc/shaders/test/frag.spv");
        Renderer renderer(window, testShader);

        this_thread::sleep_for(5s);
    } catch (const exception& e) {
        logger.logError(e.what());
    }

    terminateVideo();

    return 0;
}
