#include <iostream>
#include "source/VCTSystem.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

int main()
{
    VCTSystem::ptr app = VCTSystem::getSingleton();

    app->setWindowSize(SCR_WIDTH, SCR_HEIGHT);
    app->setTitle("voxel cone tracing");
    app->run();

    return 0;
}
