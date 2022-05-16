cmake_minimum_required(VERSION 3.8)
project(ipc_network LANGUAGES CXX)

include_directories(${CMAKE_CURRENT_LIST_DIR}/source)

add_catalog_recursive(${CMAKE_CURRENT_LIST_DIR}/source source PARSED_SOURCE)
add_library(${PROJECT_NAME} SHARED ${PARSED_SOURCE})
add_precompiled_header(${PROJECT_NAME} "pch.h")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "shared")

target_link_libraries(${PROJECT_NAME} PUBLIC logger config data_models CONAN_PKG::boost)

format_astyle()