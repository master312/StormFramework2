set(LIBS_PATH ${STORM_ROOT_DIR}/libs)
unset(STORM_LIBS)
unset(STORM_LIBS_SOURCE_FILES)

include_directories(${LIBS_PATH}/)

######## Find lua 5.1 ########
FIND_PACKAGE(Lua 5.1 REQUIRED)

if (LUA_FOUND)
    message(STATUS "Lua found at: '${LUA_INCLUDE_DIR}'")
    include_directories(${LUA_INCLUDE_DIR})
    set(STORM_LIBS ${STORM_LIBS} ${LUA_LIBRARIES})
else()
    message(FATAL_ERROR "Lua 5.1 NOT found!")
endif()

######## Find OpenGL libs ########
find_package(OpenGL REQUIRED)
if (OpenGL_FOUND)
    message(STATUS "OpenGL libs found.")
    include_directories(${OPENGL_INCLUDE_DIRS})
    set(STORM_LIBS ${STORM_LIBS} ${OPENGL_LIBRARIES})
else()
    message(FATAL_ERROR "OpenGL NOT found!")
endif()

######## Find GLEW libs ########
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    message(STATUS "GLEW libs found.")
    include_directories(${GLEW_INCLUDE_DIRS})
    set(STORM_LIBS ${STORM_LIBS} ${GLEW_LIBRARIES})
else()
    message(FATAL_ERROR "GLEW libs not found")
endif()

######## Find SDL2 libs ########
find_package(SDL2 REQUIRED)
if (SDL2_FOUND)
    message(STATUS "SDL2 libs found.")
    include_directories(${SDL2_INCLUDE_DIR})
    set(STORM_LIBS ${STORM_LIBS} ${SDL2_LIBRARY})
else()
    message(FATAL_ERROR "SDL2 not found!")
endif()
find_package(SDL2_image REQUIRED)
if (SDL2_IMAGE_FOUND)
    message(STATUS "SDL2 image libs found.")
    include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
    set(STORM_LIBS ${STORM_LIBS} ${SDL2_IMAGE_LIBRARIES})
else()
    message(FATAL_ERROR "SDL2 image libs not found!")
endif()

######## Easy logging lib ########
set(STORM_LIBS_SOURCE_FILES ${STORM_LIBS_SOURCE_FILES} ${LIBS_PATH}/easyloggingpp/easylogging++.cpp)

######## PUGI xml lib ########
set(STORM_LIBS_SOURCE_FILES ${STORM_LIBS_SOURCE_FILES} ${LIBS_PATH}/pugixml/pugixml.cpp)

######## Box2D libs ########
set(LIB_BOX2D_ROOT "${LIBS_PATH}/Box2D")
include(${LIBS_PATH}/Box2D/CMakeLists.txt)
set(STORM_LIBS ${STORM_LIBS} ${BOX2D_LIB})