set(TEST_CLIENTS_INSTALL_DIR "share/embedded-compositor-testclients")

set(CMAKE_INSTALL_RPATH "$ORIGIN/../../lib")

add_subdirectory(leftclient)
add_subdirectory(rightclient)
add_subdirectory(topclient)
add_subdirectory(bottomclient)
add_subdirectory(quickcenterclient)
add_subdirectory(widgetcenterclient)
