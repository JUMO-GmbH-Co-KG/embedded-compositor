cmake_minimum_required (VERSION 3.22)

project(embedded-compositor LANGUAGES CXX)
find_package(Qt6 REQUIRED COMPONENTS Core Quick)

qt_add_executable(${PROJECT_NAME}
    configurationhive.cpp configurationhive.h
    dbus-selector.h
    dbus/CompositorScreenDBusInterface.cpp dbus/CompositorScreenDBusInterface.hpp
    dbus/DBusInterface.cpp dbus/DBusInterface.hpp
    dbus/GlobalOverlayDBusInterface.cpp dbus/GlobalOverlayDBusInterface.hpp
    dbus/NotificationModel.cpp dbus/NotificationModel.hpp
    dbus/ScreenShotDBusInterface.cpp dbus/ScreenShotDBusInterface.hpp
    dbus/TaskSwitcherDBusInterface.cpp dbus/TaskSwitcherDBusInterface.hpp
    dbus/TaskSwitcherEntry.hpp
    embeddedshellextension.cpp embeddedshellextension.h
    sortfilterproxymodel.cpp sortfilterproxymodel.h
    main.cpp
)

qt6_generate_wayland_protocol_server_sources(${PROJECT_NAME}
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/../protocol/embedded-shell.xml
)

qt_add_dbus_adaptor(DBUS_ADAPTER_SRC 
    ../dbus/de.EmbeddedCompositor.globaloverlay.xml 
    ./dbus/GlobalOverlayDBusInterface.hpp 
    GlobalOverlayDBusInterface
    globaloverlay_adaptor
)

qt_add_dbus_adaptor(DBUS_ADAPTER_SRC 
    ../dbus/de.EmbeddedCompositor.screen.xml
    ./dbus/CompositorScreenDBusInterface.hpp
    CompositorScreenDBusInterface
    screen_adaptor
)

qt_add_dbus_adaptor(DBUS_ADAPTER_SRC 
    ../dbus/org.freedesktop.Notifications.xml
    ./dbus/NotificationModel.hpp 
    NotificationModel
    notifications_adaptor
)

qt_add_dbus_adaptor(DBUS_ADAPTER_SRC 
    ../dbus/de.EmbeddedCompositor.screenshot.xml
    ./dbus/ScreenShotDBusInterface.hpp
    ScreenShotDBusInterface
    screenshot_adaptor
)

qt_add_dbus_adaptor(DBUS_ADAPTER_SRC 
    ../dbus/de.EmbeddedCompositor.taskswitcher.xml
    ./dbus/TaskSwitcherDBusInterface.hpp
    TaskSwitcherDBusInterface
    taskswitcher_adaptor    
)

target_sources(${PROJECT_NAME} PRIVATE
    ${DBUS_ADAPTER_SRC}
)

message(STATUS ${GLOBALOVERLAY_SRC})

target_include_directories(${PROJECT_NAME} PRIVATE
    dbus
)

execute_process(
    COMMAND git describe --tags --always
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_DESCRIBE
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    EMBEDDED_COMPOSITOR_VERSION="${GIT_DESCRIBE}"
)

message(STATUS "Version: ${GIT_DESCRIBE}")

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt::Core
    Qt::CorePrivate
    Qt::DBus
    Qt::Gui
    Qt::GuiPrivate
    Qt::Quick
    Qt::Svg
    Qt::VirtualKeyboard
    Qt::WaylandCompositor
    Qt::WaylandCompositorPrivate
    embeddedplatform
)


# Resources:
set(qml_resource_files
    "qml/DefaultGlobalOverlay/GlobalOverlay.qml"
    "qml/DefaultScreenSaver/ScreenSaver.qml"
    "qml/DefaultTaskSwitcher/TaskSwitcher.qml"
    "qml/Keyboard.qml"
    "qml/Notifications/Notification.qml"
    "qml/Notifications/Notifications.qml"
    "qml/RootTransformer.qml"
    "qml/ScreenSaverController.qml"
    "qml/main.qml"
)

qt_add_resources(${PROJECT_NAME} "qml"
    PREFIX
        "/"
    FILES
        ${qml_resource_files}
)

if(use_system_bus)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        USE_SYSTEM_BUS=1
    )
endif()

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT Runtime
)
