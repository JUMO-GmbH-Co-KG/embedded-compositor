#!/bin/bash

if [ $(arch) = "armv7l" ]; then
    echo "running on device"
	export QT_WAYLAND_CLIENT_BUFFER_INTEGRATION=linux-dmabuf-unstable-v1
	export QT_QPA_PLATFORM=eglfs
	export XDG_RUNTIME_DIR=/run/user/0
	BUILD_ROOT=../build-jupiter
else
    echo "running on dev host"
	BUILD_ROOT=/home/florian/basyskom/build-compositor-Desktop-Debug
	export QT_SCREEN_SCALE_FACTORS=
	export QT_AUTO_SCREEN_SCALE_FACTOR=0
	mkdir -p $BUILD_ROOT/plugins/wayland-shell-integration
	ln -sf $BUILD_ROOT/shellintegration/libshellintegration.so $BUILD_ROOT/plugins/wayland-shell-integration
	export QT_PLUGIN_PATH=$BUILD_ROOT/plugins
fi

$BUILD_ROOT/compositor/compositor &
sleep 1

export QT_QPA_PLATFORM=wayland
export QT_WAYLAND_SHELL_INTEGRATION=embedded-shell
#export QT_PLUGIN_PATH=$BUILD_ROOT/plugins
export QML2_IMPORT_PATH=$BUILD_ROOT/quickembeddedshellwindow
#export QML_IMPORT_TRACE=1
#export QT_DEBUG_PLUGINS=1
#export WAYLAND_DEBUG=1
export LD_LIBRARY_PATH=$BUILD_ROOT/quickembeddedshellwindow/EmbeddedShell:$BUILD_ROOT/embeddedplatform

echo "======\nLaunching Clients...\n======"
$BUILD_ROOT/testclients/leftclient/leftclient &
$BUILD_ROOT/testclients/rightclient/rightclient &
$BUILD_ROOT/testclients/topclient/topclient &
$BUILD_ROOT/testclients/bottomclient/bottomclient &
#$BUILD_ROOT/testclients/quickcenterclient/quickcenterclient &
$BUILD_ROOT/testclients/widgetcenterclient/widgetcenterclient &
#gdb $BUILD_ROOT/testclients/widgetcenterclient/widgetcenterclient
wait


