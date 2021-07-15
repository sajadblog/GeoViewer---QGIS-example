cmake_minimum_required(VERSION 3.1)
project(TestGeoViewer)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/lib)

set(
    LIBRARIES
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Positioning
    Qt5::Xml
    Qt5::Sql
    qgis_gui
    qgis_core
#    qgis_app
    GeoViewer
    )

## Add search folder for CMake ------------------------------------------------------
#set(CMAKE_PREFIX_PATH $ENV{QT_CMAKE_PATH})

# Qt Dependencies -------------------------------------------------------------
find_package(Qt5 COMPONENTS Core Gui Widgets Positioning Xml Sql REQUIRED)

set(GISTEST_SRCS
    main.cpp
    mainwindow.cpp
    )

set( GISTEST_HDR
    mainwindow.h
    )
set(
    GISTEST_UI_FILES
    mainwindow.ui
    )
qt5_wrap_ui (GISTEST_UI_FILES_MOC  ${GISTEST_UI_FILES})

set(
    RESOURCE
    icons.qrc
    )

qt5_add_resources(RESOURCE_ADDED ${RESOURCE})

# QGIS Dependencies -------------------------------------------------------------
file(GLOB_RECURSE QGIS_FILESs "qgis/*.cpp" "qgis/*.h")
file(GLOB_RECURSE QGIS_UIs "qgis/*.ui" )
qt5_wrap_ui (QGIS_WRAP_UIs  ${QGIS_UIs})

include_directories(
    ../../
    )

link_directories (${CMAKE_SOURCE_DIR}/bin/lib/)

add_executable(${PROJECT_NAME} ${GISTEST_SRCS} ${GISTEST_HDR} ${GISTEST_UI_FILES_MOC} ${RESOURCE_ADDED} ${QGIS_FILESs} ${QGIS_WRAP_UIs})

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
