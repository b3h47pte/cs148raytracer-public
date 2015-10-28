#pragma once

#include "common/common.h"
#include <chrono>

class Timer
{
public:
    Timer(const std::string& descriptor);
    ~Timer();

    // Starts the timer
    void Tick();

    // Ends the timer
    void Tock();
private:
    std::string storedDescriptor;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    bool tickHandled;
};