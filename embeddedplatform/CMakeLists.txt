cmake_minimum_required(VERSION 3.22)

project(embeddedplatform VERSION 1.0.0 LANGUAGES CXX)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core DBus)

qt_add_library(${PROJECT_NAME}
    embeddedcompositordbusclient.cpp embeddedcompositordbusclient.h
    embeddedplatform.cpp embeddedplatform.h
    embeddedshell.cpp embeddedshell.h
    embeddedshellanchor.h
    embeddedshellsurface.cpp embeddedshellsurface.h embeddedshellsurface_p.h
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

qt_add_dbus_interface(DBUS_INTERFACE_SCR
    ../dbus/de.EmbeddedCompositor.globaloverlay.xml
    globaloverlay_interface
)

qt_add_dbus_interface(DBUS_INTERFACE_SCR
    ../dbus/org.freedesktop.Notifications.xml
    notifications_interface
)

qt_add_dbus_interface(DBUS_INTERFACE_SCR
    ../dbus/de.EmbeddedCompositor.screen.xml
    screen_interface
)

set_source_files_properties(../dbus/de.EmbeddedCompositor.taskswitcher.xml
    PROPERTIES
    INCLUDE ${CMAKE_SOURCE_DIR}/embedded-compositor/dbus/TaskSwitcherEntry.hpp
)

qt_add_dbus_interface(DBUS_INTERFACE_SCR
    ../dbus/de.EmbeddedCompositor.taskswitcher.xml
    taskswitcher_interface
)

target_sources(${PROJECT_NAME} PRIVATE
    ${DBUS_INTERFACE_SCR}
)

qt_generate_wayland_protocol_client_sources(${PROJECT_NAME}
    FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/../protocol/embedded-shell.xml
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    EMBEDDEDPLATFORM_LIBRARY
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
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
