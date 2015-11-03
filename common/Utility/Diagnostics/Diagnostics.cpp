#include "common/common.h"
#include "common/Utility/Diagnostics/Diagnostics.h"

#if DIAGNOSTICS_ON

Diagnostics* Diagnostics::Get()
{
    static std::unique_ptr<Diagnostics> singleton = make_unique<Diagnostics>();
    return singleton.get();
}

Diagnostics::Diagnostics()
{
}

void Diagnostics::IncrementStat(DiagnosticsType type)
{
    ++statisticsAggregator[type];
}

void Diagnostics::Log(const std::string& log)
{
    std::cout << log << std::endl;
}

void Diagnostics::Print()
{
    std::cout << "====================== DIAGNOSTICS START ======================" << std::endl;
    std::cout << "Ray-Triangle Intersections: " << statisticsAggregator[DiagnosticsType::TRIANGLE_INTERSECTIONS] << std::endl;
    std::cout << "Ray-Box Intersections: " << statisticsAggregator[DiagnosticsType::BOX_INTERSECTIONS] << std::endl;
    std::cout << "Rays Created: " << statisticsAggregator[DiagnosticsType::RAYS_CREATED] << std::endl;
    std::cout << "====================== DIAGNOSTICS END ========================" << std::endl;
}

#endif