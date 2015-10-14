#pragma once

#include "common/common.h"

class RayTracer {
public:
    RayTracer(std::unique_ptr<class Application> app);

    void Run();
private:
    std::unique_ptr<class Application> storedApplication;
};