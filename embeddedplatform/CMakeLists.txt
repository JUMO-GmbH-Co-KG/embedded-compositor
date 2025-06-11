cmake_minimum_required(VERSION 3.22)

project(embeddedplatform LANGUAGES CXX)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)

qt_add_library(${PROJECT_NAME}
    embeddedcompositordbusclient.cpp embeddedcompositordbusclient.h
    embeddedplatform.cpp embeddedplatform.h
    embeddedshell.cpp embeddedshell.h
    embeddedshellanchor.h
    embeddedshellsurface.cpp embeddedshellsurface.h embeddedshellsurface_p.h
    taskswitcher_interface.cpp taskswitcher_interface.h
    notifications_interface.cpp notifications_interface.h
    globaloverlay_interface.cpp globaloverlay_interface.h
    screen_interface.cpp screen_interface.h
    wayland-embedded-shell-protocol.c wayland-embedded-shell-client-protocol.h
    qwayland-embedded-shell.cpp qwayland-embedded-shell.h
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION 1.0.0
    VERSION 1.0
    SOVERSION 1
)

qt6_generate_wayland_protocol_client_sources(${PROJECT_NAME}
    FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/../protocol/embedded-shell.xml
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    EMBEDDEDPLATFORM_LIBRARY
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt::Core
    Qt::DBus
    Qt::Gui
    Qt::GuiPrivate
    Qt::Quick
    Qt::WaylandClientPrivate
    Wayland::Client
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

set(INTERFACE_HEADERS
    embeddedplatform.h
    embeddedshellanchor.h
    embeddedshellsurface.h
    embeddedcompositordbusclient.h)

install(FILES ${INTERFACE_HEADERS}
    DESTINATION include/embedded-compositor
)
