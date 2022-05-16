cmake_minimum_required(VERSION 3.8)
project(conan_wrapper CXX)

include(conan.cmake)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/memsharded/cmake-conan")
    file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/master/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

