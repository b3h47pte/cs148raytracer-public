cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("cs148-raytracer")

# Check system
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    MESSAGE(STATUS "64 bits compiler detected" )
    SET( EX_PLATFORM 64 )
    SET( EX_PLATFORM_NAME "x64" )
    SET( EX_PLATFORM_STR "64" )
else( CMAKE_SIZEOF_VOID_P EQUAL 8 ) 
    MESSAGE(STATUS "32 bits compiler detected" )
    SET( EX_PLATFORM 32 )
    SET( EX_PLATFORM_NAME "x86" )
    SET( EX_PLATFORM_STR "32" )
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

# Import utility cmake scripts...mainly useful for Linux systems since we provide
# the external libraries elsewhere.
if (NOT WIN32 AND NOT APPLE)
    set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
endif()

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

# CPP Flags
if (NOT WIN32)
	set(CXX_FLAGS "-Wall -std=c++11 -Wno-missing-braces")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Common Files necessary for all assignments
include_directories("./")
file(GLOB_RECURSE COMMON_SOURCES "common/*.cpp")
file(GLOB_RECURSE COMMON_HEADERS "common/*.h")

# GLM Headers
include_directories("./external/glm")

# Kd-Tree Library
include_directories("./external/libkdtree")

# Open Asset Import Headers
include_directories("./external/assimp/include")

# FreeImage Header
if (WIN32)
    include_directories("./external/freeimage/distrib/windows/${EX_PLATFORM_NAME}")
elseif (APPLE)
    include_directories("./external/freeimage/distrib/osx")
else()
    find_package(FreeImage REQUIRED)
    include_directories(${FREEIMAGE_INCLUDE_DIRS})
endif()

file(GLOB_RECURSE ASSIGNMENT_SOURCES "./assignment*/*.cpp")
file(GLOB_RECURSE ASSIGNMENT_HEADERS "./assignment*/*.h")

file(GLOB_RECURSE INSTRUCTOR_SOURCES "./instructor/*.cpp")
file(GLOB_RECURSE INSTRUCTOR_HEADERS "./instructor/*.h")

# Path to Assets
add_definitions("-DASSET_PATH=${CMAKE_CURRENT_SOURCE_DIR}/assets")

# Note that file globbing is generally not recommended. It is usually better to explicitly list files; however,
# I am assuming that most students are unfamiliar with CMake and thus will not be able to update the CMakeLists.txt
# file appropriately. Hence, globbing is necessary for me to generate the Makefiles/whatever again should I ever
# want to run their code.
add_definitions(${CXX_FLAGS})
add_executable(cs148raytracer main.cpp ${COMMON_SOURCES} ${COMMON_HEADERS}
    ${ASSIGNMENT_SOURCES} ${ASSIGNMENT_HEADERS} ${INSTRUCTOR_SOURCES} ${INSTRUCTOR_HEADERS})

# Open Asset Import Library
if (WIN32)
    target_link_libraries(cs148raytracer "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/distrib/windows/lib${EX_PLATFORM_STR}/assimp.lib")
elseif (APPLE)
    target_link_libraries(cs148raytracer "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/distrib/osx/libassimp.dylib")
else()
    target_link_libraries(cs148raytracer "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/distrib/unix/libassimp.so")
endif()

# FreeImage Library
if (WIN32)
	target_link_libraries(cs148raytracer "${CMAKE_CURRENT_SOURCE_DIR}/external/freeimage/distrib/windows/${EX_PLATFORM_NAME}/FreeImage.lib")
elseif (APPLE)
    target_link_libraries(cs148raytracer "${CMAKE_CURRENT_SOURCE_DIR}/external/freeimage/distrib/osx/libfreeimage.a")
else()
    target_link_libraries(cs148raytracer ${FREEIMAGE_LIBRARY})
endif()

# Source Files
source_group(common REGULAR_EXPRESSION common/.*)
source_group(common\\Acceleration REGULAR_EXPRESSION common/Acceleration/.*)
source_group(common\\Acceleration\\BVH REGULAR_EXPRESSION common/Acceleration/BVH/.*)
source_group(common\\Acceleration\\Naive REGULAR_EXPRESSION common/Acceleration/Naive/.*)
source_group(common\\Acceleration\\UniformGrid REGULAR_EXPRESSION common/Acceleration/UniformGrid/.*)
source_group(common\\Intersection REGULAR_EXPRESSION common/Intersection/.*)
source_group(common\\Output REGULAR_EXPRESSION common/Output/.*)
source_group(common\\Rendering REGULAR_EXPRESSION common/Rendering/.*)
source_group(common\\Rendering\\Textures REGULAR_EXPRESSION common/Rendering/Textures/.*)
source_group(common\\Rendering\\Material REGULAR_EXPRESSION common/Rendering/Material/.*)
source_group(common\\Rendering\\Material\\BlinnPhong REGULAR_EXPRESSION common/Rendering/Material/BlinnPhong/.*)
source_group(common\\Rendering\\Renderer REGULAR_EXPRESSION common/Rendering/Renderer/.*)
source_group(common\\Rendering\\Renderer\\Backward REGULAR_EXPRESSION common/Rendering/Renderer/Backward/.*)
source_group(common\\Rendering\\Renderer\\Photon REGULAR_EXPRESSION common/Rendering/Renderer/Photon/.*)
source_group(common\\Sampling REGULAR_EXPRESSION common/Sampling/.*)
source_group(common\\Sampling\\Adaptive REGULAR_EXPRESSION common/Sampling/Adaptive/.*)
source_group(common\\Sampling\\Adaptive\\Simple REGULAR_EXPRESSION common/Sampling/Adaptive/Simple/.*)
source_group(common\\Sampling\\Jitter REGULAR_EXPRESSION common/Sampling/Jitter/.*)
source_group(common\\Scene REGULAR_EXPRESSION common/Scene/.*)
source_group(common\\Scene\\Camera REGULAR_EXPRESSION common/Scene/Camera/.*)
source_group(common\\Scene\\Camera\\Perspective REGULAR_EXPRESSION common/Scene/Camera/Perspective/.*)
source_group(common\\Scene\\Geometry REGULAR_EXPRESSION common/Scene/Geometry/.*)
source_group(common\\Scene\\Geometry\\Mesh REGULAR_EXPRESSION common/Scene/Geometry/Mesh/.*)
source_group(common\\Scene\\Geometry\\Primitives REGULAR_EXPRESSION common/Scene/Geometry/Primitves/.*)
source_group(common\\Scene\\Geometry\\Primitives\\Triangle REGULAR_EXPRESSION common/Scene/Geometry/Primitves/Triangle/.*)
source_group(common\\Scene\\Geometry\\Ray REGULAR_EXPRESSION common/Scene/Geometry/Ray/.*)
source_group(common\\Scene\\Geometry\\Simple REGULAR_EXPRESSION common/Scene/Geometry/Simple/.*)
source_group(common\\Scene\\Geometry\\Simple\\Box REGULAR_EXPRESSION common/Scene/Geometry/Simple/Box/.*)
source_group(common\\Scene\\Lights REGULAR_EXPRESSION common/Scene/Lights/.*)
source_group(common\\Scene\\Lights\\Directional REGULAR_EXPRESSION common/Scene/Lights/Directional/.*)
source_group(common\\Scene\\Lights\\Point REGULAR_EXPRESSION common/Scene/Lights/Point/.*)
source_group(common\\Utility REGULAR_EXPRESSION common/Utility/.*)
source_group(common\\Utility\\Diagnostics REGULAR_EXPRESSION common/Utility/Diagnostics/.*)
source_group(common\\Utility\\Texture REGULAR_EXPRESSION common/Utility/Texture/.*)
source_group(common\\Utility\\Mesh REGULAR_EXPRESSION common/Utility/Mesh/.*)
source_group(common\\Utility\\Mesh\\Loading REGULAR_EXPRESSION common/Utility/Mesh/Loading/.*)
source_group(common\\Utility\\Timer REGULAR_EXPRESSION common/Utility/Timer/.*)

# Copy dlls
if (WIN32)
	add_custom_command(TARGET cs148raytracer POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/distrib/windows/bin${EX_PLATFORM_STR}/assimp.dll" "$<TARGET_FILE_DIR:cs148raytracer>")
	add_custom_command(TARGET cs148raytracer POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/external/freeimage/distrib/windows/${EX_PLATFORM_NAME}/FreeImage.dll" "$<TARGET_FILE_DIR:cs148raytracer>")
endif()
