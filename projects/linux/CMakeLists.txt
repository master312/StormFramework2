cmake_minimum_required(VERSION 3.2)
project(StormEngine2)

set(STORM_ROOT_DIR ${PROJECT_SOURCE_DIR}/../../)
set(STORM_SRC_DIR ${PROJECT_SOURCE_DIR}/../../src)

# Set default configuration variables, for cmake-gui
set(STORM_BUILD_SDL2_SUPPORT true CACHE BOOL "")


# Enable C++14 standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include main engine sources
include(${STORM_ROOT_DIR}/CMakeLists.txt)

# Sets binary output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${STORM_ROOT_DIR}/bin)
add_executable(${PROJECT_NAME} ${STORM_SOURCE_FILES} main.cpp)

# Set libraries
target_link_libraries(${PROJECT_NAME} ${STORM_LIBS})

# Set compiler parameters
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

# Set preprocessor definitions
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS ${STORM_PREPROCESSOR_FLAGS})

# Include editor configuration
# REMAINDER: Comment-out this to prevent editor from building
# include(${STORM_ROOT_DIR}/src_editor/CMakeLists.txt)
