cmake_minimum_required (VERSION 3.22)

project(quickembeddedshellwindow LANGUAGES CXX)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)

qt_add_library(${PROJECT_NAME}
    quickembeddedshell.cpp quickembeddedshell.h
    quickembeddedshellwindow.cpp quickembeddedshellwindow.h
    quickembeddedshellwindow_global.h
    waylandinputregion.cpp waylandinputregion.h
)
target_compile_definitions(${PROJECT_NAME} PUBLIC
    QUICKEMBEDDEDSHELLWINDOW_LIBRARY
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt::Core
    Qt::Quick
    Qt::WaylandClientPrivate
    embeddedplatform
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
