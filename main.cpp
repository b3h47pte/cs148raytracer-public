#include "common/RayTracer.h"

#define ASSIGNMENT 8
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

#ifdef _WIN32
#define WAIT_ON_EXIT 1
#else
#define WAIT_ON_EXIT 0
#endif

int main(int argc, char** argv)  
{
    std::unique_ptr<APPLICATION> currentApplication = make_unique<APPLICATION>();
    RayTracer rayTracer(std::move(currentApplication));

    DIAGNOSTICS_TIMER(timer, "Ray Tracer");
    rayTracer.Run();
    DIAGNOSTICS_END_TIMER(timer);

    DIAGNOSTICS_PRINT();

#if defined(_WIN32) && WAIT_ON_EXIT
    int exit = 0;
    std::cin >> exit;
#endif

    return 0;
}
