#include "engine/Engine.h"

int main() {
    Engine engine;
    if (!engine.init(1280, 720, "VoxelCraft")) {
        return 1;
    }

    engine.run();
    engine.shutdown();
    return 0;
}
