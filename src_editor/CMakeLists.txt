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
    ${SOURCES_PATH_PREFIX}/colorswatch.h
    ${SOURCES_PATH_PREFIX}/colorswatch.cpp
    ${SOURCES_PATH_PREFIX}/toolbar.h
    ${SOURCES_PATH_PREFIX}/toolbar.cpp
    ${SOURCES_PATH_PREFIX}/MainWindow.h
    ${SOURCES_PATH_PREFIX}/MainWindow.cpp

    ${SOURCES_PATH_PREFIX}/toolbars/SEFileToolbar.h
    ${SOURCES_PATH_PREFIX}/toolbars/SEFileToolbar.cpp
    ${SOURCES_PATH_PREFIX}/dock_widgets/SEDockWidget.cpp
    ${SOURCES_PATH_PREFIX}/dock_widgets/SEDockWidget.h
    ${SOURCES_PATH_PREFIX}/toolbars/SFEditToolbar.cpp
    ${SOURCES_PATH_PREFIX}/toolbars/SFEditToolbar.h
    ${SOURCES_PATH_PREFIX}/toolbars/SEToolbar.cpp
    ${SOURCES_PATH_PREFIX}/toolbars/SEToolbar.h
    )

qt5_add_resources(UI_RESOURCES ${SOURCES_PATH_PREFIX}/resources.qrc)

add_executable(${PROJECT_NAME} ${engine_sources} ${editor_sources} ${UI_RESOURCES} )

# include /src_editor directory
target_include_directories(${PROJECT_NAME} PUBLIC ${SOURCES_PATH_PREFIX})
# Add ELPP_QT_LOGGING define for EasyLoggin API
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DELPP_QT_LOGGING")

target_link_libraries(${PROJECT_NAME}
        ${STORM_LIBS}
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui
        )