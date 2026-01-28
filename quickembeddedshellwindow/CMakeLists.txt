cmake_minimum_required (VERSION 3.22)

project(quickembeddedshellwindow VERSION 1.0.0 LANGUAGES CXX)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)

qt_add_library(${PROJECT_NAME}
    quickembeddedshell.cpp quickembeddedshell.h
    quickembeddedshellwindow.cpp quickembeddedshellwindow.h
    quickembeddedshellview.cpp quickembeddedshellview.h
    quickembeddedshellwindow_global.h
    waylandinputregion.cpp waylandinputregion.h
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
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

set(INSTALL_PATH ${CMAKE_INSTALL_LIBDIR}/qml/EmbeddedShell/)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${INSTALL_PATH}
)

install(FILES qmldir
    DESTINATION ${INSTALL_PATH}
)
