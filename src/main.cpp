#include <iostream>

#include <vulkan/vulkan.h>

#include "engine/subspace.hpp"
#include "generated/default_config.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    subspace::Config config {"rc/config.json"};
    config.setFullscreen(!config.isFullscreen());

    return 0;
}
