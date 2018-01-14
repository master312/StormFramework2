project(StormEngine2_Editor)
set(SOURCES_PATH_PREFIX src_editor)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

#find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt5Widgets CONFIG
        PATHS /home/master312/Documents/Qt-Suite/5.9/gcc_64/lib/cmake/Qt5Widgets
        NO_DEFAULT_PATH)

list(APPEND THIRD_PARTY_LIBS Qt5::Widgets Qt5::Core)

# Create sources list
set(editor_sources
    ${SOURCES_PATH_PREFIX}/main.cpp
    ${SOURCES_PATH_PREFIX}/mainwindow.h
    ${SOURCES_PATH_PREFIX}/mainwindow.cpp
    )


add_executable(${PROJECT_NAME} ${engine_sources} ${editor_sources})

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DELPP_QT_LOGGING")

target_link_libraries(${PROJECT_NAME}
        ${STORM_LIBS}
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui
        )