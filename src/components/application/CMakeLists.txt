cmake_minimum_required(VERSION 3.8)
project(application LANGUAGES CXX)

include_directories(${CMAKE_CURRENT_LIST_DIR}/source)
add_catalog_recursive(${CMAKE_CURRENT_LIST_DIR}/source source PARSED_SOURCE)
add_executable(${PROJECT_NAME} ${PARSED_SOURCE})
target_link_libraries(${PROJECT_NAME} PUBLIC AppCore Ultralight UltralightCore WebCore CONAN_PKG::glfw CONAN_PKG::glad ui)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "shared")

add_ultralight()

format_astyle()