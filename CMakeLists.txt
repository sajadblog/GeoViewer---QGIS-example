cmake_minimum_required(VERSION 3.1)
project(GeoViewer)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/lib)

# set path to additional CMake modules
#SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})


option(TestCompile "if checked test Application will be compiled" True)

set(
    LIBRARIES
    qgis_gui
    qgis_core
#    qgis_app
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Positioning
    Qt5::Xml
    Qt5::Network
    Qt5::Sql
    Qt5::Svg
    QDateConvertor
    )

## Add search folder for CMake ------------------------------------------------------
#set(CMAKE_PREFIX_PATH $ENV{QT_CMAKE_PATH})

# Qt Dependencies -------------------------------------------------------------
find_package(Qt5 COMPONENTS Core Gui Widgets Positioning Xml Network Sql Svg REQUIRED)
# Dependencies -------------------------------------------------------------
## QGIS Dependencies -------------------------------------------------------------

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DQGISDEBUG)
ENDIF()

file(GLOB_RECURSE QGIS_FILES "qgis/*.cpp" "qgis/*.h")
file(GLOB_RECURSE QGIS_UI "qgis/*.ui" )
qt5_wrap_ui (QGIS_WRAP_UI  ${QGIS_UI})

#add_definitions(-DQT_NO_SSL) # remove qca dependence
#add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0)
## Password helper
#FIND_PACKAGE(QtKeychain REQUIRED)

##third party
if(NOT TARGET QDateConvertor)
    add_subdirectory(lib/qdateconvertor)
endif()

#----------------------------------------------- GIS Engine Files
set(GIS_SRCS
    src/mapwidget.cpp
    src/common/geodatacontainer.cpp
    src/common/geounitmanager.cpp
    src/widgets/geomapcanvas.cpp
    src/items/geoitem.cpp
    src/items/geomarkeritem.cpp
    src/items/geotextitem.cpp
    src/items/geomultipositionItem.cpp
    src/items/geosimplemarkeritem.cpp
    src/items/geosectoritem.cpp
    src/items/geocolorizepathitem.cpp
    #----------------------------Components
    src/components/geocomponent.cpp
    src/components/geobrowsercomponent.cpp
    src/components/geolayerscomponent.cpp
    #----------------------------Tools
    src/tools/geomultipositiontool.cpp
    src/tools/geo2positionitemtool.cpp
    src/tools/geomeasuretool.cpp
    )

set( GIS_HDR
    GeoViewer/mapwidget.h
    GeoViewer/widgets/geomapcanvas.h
    GeoViewer/common/mapstructures.h
    GeoViewer/common/mapenums.h
    GeoViewer/common/geodatacontainer.h
    GeoViewer/common/geounitmanager.h
    GeoViewer/items/geoitem.h
    GeoViewer/items/geomarkeritem.h
    GeoViewer/items/geotextitem.h
    GeoViewer/items/geomultipositionItem.h
    GeoViewer/items/geosimplemarkeritem.h
    GeoViewer/items/geosectoritem.h
    GeoViewer/items/geocolorizepathitem.h
    #----------------------------Components
    GeoViewer/components/geocomponent.h
    GeoViewer/components/geobrowsercomponent.h
    GeoViewer/components/geolayerscomponent.h
    #----------------------------Tools
    GeoViewer/tools/geomultipositiontool.h
    GeoViewer/tools/geo2positionitemtool.h
    GeoViewer/tools/geomeasuretool.h
    )
set(
    GIS_UI_FILES
    ui/mapwidget.ui
    )
qt5_wrap_ui (GIS_WRAP_UI  ${GIS_UI_FILES})


set(
    RESOURCE
    qrc/images/images.qrc
    )
qt5_add_resources(GIS_RESOURCE_ADDED ${RESOURCE})


include_directories(
    GeoViewer
    lib/qdateconvertor
    /usr/include/qgis
    /usr/include/qt5keychain/
    /usr/include/gdal/
    )

set(ALL_FILES
    ${GIS_SRCS}
    ${GIS_HDR}
    ${GIS_WRAP_UI}
    ${GIS_RESOURCE_ADDED}
    ${QGIS_FILES}
    ${QGIS_WRAP_UI}
    )


add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})


if(TestCompile)
    add_subdirectory(test/GeoViewerTest)
endif()


