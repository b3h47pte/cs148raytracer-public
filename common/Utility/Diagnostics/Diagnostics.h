#pragma once

#define DIAGNOSTICS_ON 1

enum class DiagnosticsType
{
    TRIANGLE_INTERSECTIONS = 0,
    BOX_INTERSECTIONS,
    RAYS_CREATED,
    MAX
};

#if DIAGNOSTICS_ON
#define DIAGNOSTICS_STAT(t) Diagnostics::Get()->IncrementStat(t)
#define DIAGNOSTICS_PRINT() Diagnostics::Get()->Print()

#include <memory>
#include <map>

class Diagnostics
{
public:
    Diagnostics();

    static Diagnostics* Get();

    void IncrementStat(DiagnosticsType type);
    void Print();
private:

    std::map<DiagnosticsType, int> statisticsAggregator;
};

#else
#define DIAGNOSTICS_STAT(t)
#define DIAGNOSTICS_PRINT
#endif