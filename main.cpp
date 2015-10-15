#include "common/RayTracer.h"

#define ASSIGNMENT 5
#if ASSIGNMENT == 5
#define APPLICATION Assignment5
#include "assignment5/Assignment5.h"
#elif ASSIGNMENT == 6
#define APPLICATION Assignment6
#include "assignment6/Assignment6.h"
#elif ASSIGNMENT == 7
#define APPLICATION Assignment7
#include "assignment7/Assignment7.h"
#elif ASSIGNMENT == 8
#define APPLICATION Assignment8
#include "assignment8/Assignment8.h"
#endif

#include <chrono>

int main(int argc, char** argv)  
{
    std::unique_ptr<APPLICATION> currentApplication = make_unique<APPLICATION>();
    RayTracer rayTracer(std::move(currentApplication));

    std::cout << "Ray Tracer Start" << std::endl;
    auto startTime = std::chrono::high_resolution_clock::now();
    rayTracer.Run();
    auto endTime = std::chrono::high_resolution_clock::now();
    auto totalElapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
    std::cout << "Ray Tracer Elapsed Time: " << totalElapsedTime.count() << " seconds" << std::endl;

#ifdef _WIN32
    int exit = 0;
    std::cin >> exit;
#endif

    return 0;
}