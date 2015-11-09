#include "common/Utility/Timer/Timer.h"

Timer::Timer(const std::string& descriptor):
    storedDescriptor(descriptor), tickHandled(false)
{
    Tick();
}

Timer::~Timer()
{
    Tock();
}

void Timer::Tick()
{
    startTime = std::chrono::high_resolution_clock::now();
    tickHandled = false;
}

void Timer::Tock()
{
    if (tickHandled)  {
        return;
    }
    tickHandled = false;
    auto endTime = std::chrono::high_resolution_clock::now();
    auto totalElapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
    std::cout << "END " << storedDescriptor << ": " << totalElapsedTime.count() << " seconds" << std::endl;
}