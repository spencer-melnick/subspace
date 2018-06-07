#include <iostream>
#include <exception>

#include <vulkan/vulkan.h>

#include "engine/subspace.hpp"
#include "generated/default_config.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    subspace::Config config {"rc/config.json"};

    try {
        subspace::initializeVideo();
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}
