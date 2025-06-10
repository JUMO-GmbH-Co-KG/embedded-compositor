cmake_minimum_required(VERSION 3.16)
project(testclients VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup()


add_subdirectory(leftclient)
add_subdirectory(rightclient)
add_subdirectory(topclient)
add_subdirectory(bottomclient)
add_subdirectory(quickcenterclient)
add_subdirectory(widgetcenterclient)
