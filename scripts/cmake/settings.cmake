if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    if(MSVC)
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "Export all symbols")
        add_definitions(-D_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING 
                        -D_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
                        -D_WIN32_WINNT=0x0601)
        set(ENV{VCINSTALLDIR} "${CMAKE_GENERATOR_INSTANCE}/VC/")
        #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /MP")
    else()
        message(FATAL_ERROR "unknown compiler for Windows")
    endif()
endif()

cmake_host_system_information(RESULT logical_core_cout QUERY NUMBER_OF_LOGICAL_CORES)
set(CMAKE_AUTOGEN_PARALLEL ${logical_core_cout})

get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(IS_MULTI_CONFIG)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "CMAKE_CONFIGURATION_TYPES" FORCE)
elseif(NOT IS_MULTI_CONFIG)
    set(CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE} CACHE STRING "CMAKE_CONFIGURATION_TYPES" FORCE)
    set(CONAN_EXTRA_ARGS NO_OUTPUT_DIRS)
endif(IS_MULTI_CONFIG)

conan_cmake_run(CONANFILE ${CMAKE_SOURCE_DIR}/conanfile.py
                CMAKE_TARGETS BASIC_SETUP BUILD missing ${CONAN_EXTRA_ARGS})