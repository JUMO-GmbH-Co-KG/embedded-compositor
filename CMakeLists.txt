cmake_minimum_required(VERSION 3.16)

project(compositor VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 5.15 REQUIRED COMPONENTS Core Quick WaylandCompositor)


add_executable(compositor
    main.cpp
	main.qml
)

target_link_libraries(compositor PRIVATE Qt5::Core Qt5::WaylandCompositor Qt5::Quick)


add_executable(leftclient
	testclients/leftclient/main.cpp
	testclients/leftclient/qml.qrc
	testclients/leftclient/main.qml
	)
target_link_libraries(leftclient PRIVATE Qt5::Core Qt5::WaylandCompositor Qt5::Quick)


add_executable(topclient
	testclients/topclient/main.cpp
	testclients/topclient/qml.qrc
	testclients/topclient/main.qml
	)
target_link_libraries(topclient PRIVATE Qt5::Core Qt5::WaylandCompositor Qt5::Quick)
