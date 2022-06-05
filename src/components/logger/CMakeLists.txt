cmake_minimum_required(VERSION 3.8)
project(logger LANGUAGES CXX)

add_catalog_recursive(${CMAKE_CURRENT_LIST_DIR}/source source PARSED_SOURCE)
add_library(${PROJECT_NAME} SHARED ${PARSED_SOURCE})
add_precompiled_header(${PROJECT_NAME} "pch.h")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "shared")

if(WIN32)
target_link_libraries(${PROJECT_NAME} PUBLIC filesystem CONAN_PKG::boost)
endif()
if(UNIX)
target_link_libraries(${PROJECT_NAME} PUBLIC filesystem CONAN_PKG::boost)
endif()
format_astyle()