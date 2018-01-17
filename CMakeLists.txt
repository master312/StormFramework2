project(StormEngine2)
cmake_minimum_required(VERSION 3.2)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/modules")

# Set default configuration variables
set(BUILD_SDL2_SUPPORT true CACHE BOOL "")


# Sets build type if one is not set
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

message(STATUS "Build type configuration set to ${CMAKE_BUILD_TYPE}")

# Enable C++14 standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Sets binary output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

# Include libs
include(cmake/CMakeLibsSettings.cmake)

# Include sources
include(src/CMakeLists.txt)

# Create sources list
set(engine_sources
    ${STORM_ENGINE_SOURCE_FILES} # Defined in src/CMakeList.txt
    ${STORM_LIBS_SOURCE_FILES}   # Defined in CMakeLibsSettings.txt
    )



add_executable(${PROJECT_NAME} ${engine_sources} main.cpp)

# Set libraries
target_link_libraries(${PROJECT_NAME} ${STORM_LIBS})

# Set compiler parameters
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")   TODO: Debug and release configs
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} -Wall") TODO: Debug and release configs

if (BUILD_SDL2_SUPPORT)
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSTORM_BUILD_PLATFORM_SDL2")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    # Release configuration
    add_definitions("-DPRODUCTION")
else()
    # Debug configuration
    add_definitions("-D_DEBUG")
endif()


# Include editor configuration
include(src_editor/CMakeLists.txt)
















## Check for standard to use
#check_cxx_compiler_flag(-std=c++17 HAVE_FLAG_STD_CXX17)
#if(HAVE_FLAG_STD_CXX17)
#    # Have -std=c++17, use it
#else()
#    check_cxx_compiler_flag(-std=c++1z HAVE_FLAG_STD_CXX1Z)
#    if(HAVE_FLAG_STD_CXX1Z)
#        # Have -std=c++1z, use it
#    else()
#        # And so on and on...
#    endif()
#endif()