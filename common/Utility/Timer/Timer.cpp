#include "common/Utility/Timer/Timer.h"

Timer::Timer(const std::string& descriptor):
    storedDescriptor(descriptor)
{
}

void Timer::Tick()
{
    std::cout << "-- START " << storedDescriptor << " --" << std::endl;
    startTime = std::chrono::high_resolution_clock::now();
}

void Timer::Tock()
{
    auto endTime = std::chrono::high_resolution_clock::now();
    auto totalElapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
    std::cout << "-- END " << storedDescriptor << ": " << totalElapsedTime.count() << " seconds --" << std::endl;
}