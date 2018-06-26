#pragma once


/**
 * Render interface
 */
namespace subspace {
    class IRenderer {
        public:
            virtual void draw() = 0;
    };
}
