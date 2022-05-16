cmake_minimum_required(VERSION 3.8)
project(SocialNetworkManager LANGUAGES CXX)

set(THIRD_PARTY    "${CMAKE_SOURCE_DIR}/third_party/")
set(THIRD_PARTY_INCLUDE "" CACHE INTERNAL "")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(scripts/cmake/conan)

include(scripts/cmake/parse.cmake)
include(scripts/cmake/precompiled_header.cmake)
include(scripts/cmake/artifacts.cmake)
include(scripts/cmake/astyle.cmake)
include(scripts/cmake/settings.cmake)
include(scripts/cmake/ultralight.cmake)

add_subdirectory(src)