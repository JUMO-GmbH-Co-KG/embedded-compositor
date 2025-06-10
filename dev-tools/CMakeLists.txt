cmake_minimum_required(VERSION 3.16)
project(dev-tools VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6REQUIRED COMPONENTS DBus Gui Quick Widgets)

qt_standard_project_setup()


add_subdirectory(testclients)
add_subdirectory(systemd)
add_subdirectory(example-components)
