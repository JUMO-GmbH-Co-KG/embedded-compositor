cmake_minimum_required (VERSION 3.22)

project(embeddedplatform LANGUAGES CXX)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)

qt_add_library(${PROJECT_NAME}
    embeddedcompositordbusclient.cpp embeddedcompositordbusclient.h
    embeddedplatform.cpp embeddedplatform.h
    embeddedshell.cpp embeddedshell.h
    embeddedshellanchor.h
    embeddedshellsurface.cpp embeddedshellsurface.h embeddedshellsurface_p.h
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
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
