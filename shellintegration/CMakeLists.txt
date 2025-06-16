cmake_minimum_required (VERSION 3.22)

project(shellintegration LANGUAGES CXX)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS DBus Gui Quick WaylandClient)

qt_add_plugin(${PROJECT_NAME})
target_sources(shellintegration PRIVATE
    embeddedshellintegration.cpp embeddedshellintegration.h
    main.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    EMBEDDEDSHELLINTEGRATION_LIBRARY
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt::Core
    Qt::Gui
    Qt::GuiPrivate
    Qt::Quick
    Qt::WaylandClientPrivate
    embeddedplatform
)

install(TARGETS shellintegration
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/plugins/wayland-shell-integration
)
