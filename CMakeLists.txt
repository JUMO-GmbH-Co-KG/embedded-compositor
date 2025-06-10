cmake_minimum_required(VERSION 3.16)
project(embedded-compositor LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS DBus Gui Quick Svg VirtualKeyboard WaylandClient WaylandCompositor Widgets)
find_package(Wayland)

qt_standard_project_setup()


add_subdirectory(shellintegration)
add_subdirectory(embedded-compositor)
add_subdirectory(embeddedplatform)
add_subdirectory(quickembeddedshellwindow)
#add_subdirectory(dev-tools/testclients)
