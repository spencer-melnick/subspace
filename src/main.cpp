#include <iostream>

#include "engine/subspace.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    subspace::Config config {"rc/config.json"};
    cout << boolalpha;
    cout << "Fullscreen: " << config.isFullscreen() << endl;

    return 0;
}
