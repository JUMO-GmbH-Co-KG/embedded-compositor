cmake_minimum_required (VERSION 3.22)

project(bottomclient LANGUAGES CXX)
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS DBus Gui Quick)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

qt_add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt::Core
    Qt::DBus
    Qt::Gui
    Qt::Quick
    embeddedplatform
)


# Resources:
set(qml_resource_files
    "main.qml"
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
    RUNTIME DESTINATION ${TEST_CLIENTS_INSTALL_DIR}
    COMPONENT Runtime
)
