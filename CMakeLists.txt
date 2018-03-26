cmake_minimum_required(VERSION 3.2)

# Variable ${STORM_ROOT_DIR} is root path of storm engine directory (the one containing /src /bin etc...)
# Variable ${STORM_SRC_DIR} is path to storm engine's source directory
# Variable ${STORM_PREPROCESSOR_FLAGS} contains all preprocessor flags to be given to compiler

set(STORM_ADITIONAL_CMAKE_DIR ${CMAKE_MODULE_PATH} ${STORM_ROOT_DIR}/cmake/)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${STORM_ADITIONAL_CMAKE_DIR}modules)


# Sets build type if one is not set
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

message(STATUS "Build type configuration set to ${CMAKE_BUILD_TYPE}")

# Include LIBS cmake file
include(${STORM_ADITIONAL_CMAKE_DIR}/CMakeLibsSettings.cmake)


# Include engine sources
include(${STORM_SRC_DIR}/CMakeLists.txt)

# Create sources list
set(STORM_SOURCE_FILES
    ${STORM_ENGINE_SOURCE_FILES} # Defined in src/CMakeList.txt
    ${STORM_LIBS_SOURCE_FILES}   # Defined in CMakeLibsSettings.txt
    )

# Set include dirs
include_directories(${STORM_ROOT_DIR} ${STORM_SRC_DIR} ${STORM_SRC_DIR}/core)


#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")   TODO: Debug and release configs
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} -Wall") TODO: Debug and release configs

# Set compiler flags
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    # Release configuration
    add_definitions("-DPRODUCTION")
else()
    # Debug configuration
    add_definitions("-D_DEBUG")
endif()

if (STORM_BUILD_SDL2_SUPPORT)
	set(STORM_PREPROCESSOR_FLAGS "${STORM_PREPROCESSOR_FLAGS} -DSTORM_BUILD_PLATFORM_SDL2")
endif()