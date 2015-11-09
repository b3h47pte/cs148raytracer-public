#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <cassert>
#include <limits>
#include <cmath>
#include <array>
#include <functional>
#include <sstream>
#include <stdint.h>
#include <unordered_map>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/string_cast.hpp"

#include "common/Utility/Timer/Timer.h"
#include "common/Utility/Diagnostics/Diagnostics.h"

const float PI = 3.14159265359f;
const float LARGE_EPSILON = 1e-5f;
const float SMALL_EPSILON = 1e-7f;


#define STRINGIFY_HELPER(x) #x
#define STRINGIFY(x) STRINGIFY_HELPER(x)
#define DISABLE_ACCELERATION_CREATION_TIMER 1


#ifdef _WIN32
#define make_unique std::make_unique
#else
template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params)
{
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}
#endif