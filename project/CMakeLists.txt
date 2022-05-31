cmake_minimum_required(VERSION 3.5)

project(stock_prediction_app VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Boost_USE_STATIC_LIBS OFF)######
set(Boost_USE_MULTITHREADED ON)#####
set(Boost_USE_STATIC_RUNTIME OFF)#####
find_package(Boost 1.76.0)##########

SET(BOOST_INCLUDE  "C:/boost/include/boost-1_76")######
SET(BOOST_LIB "C:/boost/lib")#########


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt5 REQUIRED COMPONENTS Charts)
find_package(Qt5 REQUIRED COMPONENTS Gui)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        startwindow.cpp
        mainwindow.h
        startwindow.h
        mainwindow.ui
        startwindow.ui
        aboutwindow.h
        aboutwindow.cpp
        aboutwindow.ui
        usagewindow.h
        usagewindow.cpp
        usagewindow.ui
)

ADD_DEFINITIONS("-DBOOST_ALL_NO_LIB") ####################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${BOOST_INCLUDE} include)#########################



if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(stock_prediction_app
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET stock_prediction_app APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(stock_prediction_app SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(stock_prediction_app
            ${PROJECT_SOURCES}
        )
    endif()
endif()

#target_link_libraries(mytarget PRIVATE Qt6::Charts)

target_link_libraries(stock_prediction_app PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt5::Charts Qt5::Gui ${BOOST_LIB})
set_target_properties(stock_prediction_app PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(stock_prediction_app)
endif()
