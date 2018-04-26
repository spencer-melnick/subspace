#pragma once

#include <string>

using namespace std;

namespace subspace {
    class Config {
        public:
            Config(const string& filename);

            bool isFullscreen() const;

        private:
            bool fullscreen_;
    };
}
