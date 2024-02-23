#!/bin/bash

if [ $(arch) = "armv7l" ]; then
    echo "running on device"
	export QT_WAYLAND_CLIENT_BUFFER_INTEGRATION=linux-dmabuf-unstable-v1
	export QT_QPA_PLATFORM=eglfs
	export XDG_RUNTIME_DIR=/run/user/0
	BUILD_ROOT=../build-jupiter
else
    echo "running on dev host"
    export $(dbus-launch)
    qdbusviewer &
    BUILD_ROOT=/home/florian/basyskom/build-embedded-compositor-Desktop-Debug
	export QT_ENABLE_HIGHDPI_SCALING=0
	export QT_SCREEN_SCALE_FACTORS=
	export QT_AUTO_SCREEN_SCALE_FACTOR=0
	mkdir -p $BUILD_ROOT/plugins/wayland-shell-integration
	ln -sf $BUILD_ROOT/shellintegration/libshellintegration.so $BUILD_ROOT/plugins/wayland-shell-integration
	export QT_PLUGIN_PATH=$BUILD_ROOT/plugins

  export COMPOSITOR_screenWidth=1280
  export COMPOSITOR_screenHeight=800
  export COMPOSITOR_screenSaverEnabled=true
  export COMPOSITOR_screenSaverTimeoutSeconds=300
  export COMPOSITOR_screenSaverMouseHoverSupport=true
  export COMPOSITOR_screenOrientation=0
  set | grep DBUS
fi


#export QDBUS_DEBUG=1
export QT_LOGGING_RULES="embeddedshell.compositor.debug=false;embeddedshell.quick=false"
export COMPOSITOR_taskSwitcherUrl="file:$BUILD_ROOT/../embedded-compositor/dev-tools/example-components/GridSwitcher.qml"
#export COMPOSITOR_globalOverlayUrl="file:$BUILD_ROOT/../embedded-compositor/dev-tools/example-components/AltBootScreen.qml"
#export QT_DEBUG_PLUGINS=1
# Launch only the compositor with qtvirtualkeyboard input method.
QT_IM_MODULE=qtvirtualkeyboard $BUILD_ROOT/embedded-compositor/embedded-compositor &
compositor_pid=$!


sleep 1
if [ ! -d /proc/${compositor_pid} ]; then
    echo "compositor died, exiting script"
	exit $!
fi

export QT_QPA_PLATFORM=wayland
export QT_WAYLAND_SHELL_INTEGRATION=embedded-shell
#export QT_PLUGIN_PATH=$BUILD_ROOT/plugins
export QML2_IMPORT_PATH=$BUILD_ROOT/quickembeddedshellwindow
#export QML_IMPORT_TRACE=1
#export QT_DEBUG_PLUGINS=1
#export WAYLAND_DEBUG=1
export LD_LIBRARY_PATH=$BUILD_ROOT/quickembeddedshellwindow/EmbeddedShell:$BUILD_ROOT/embeddedplatform

echo "======  Launching Clients... ======"

# Ensure that no input module (e.g. ibus) is set for our clients.
# We want Qt Wayland Client to use its built-in zwp_text_input_v2 support.
unset QT_IM_MODULE

CLIENTS=$BUILD_ROOT/dev-tools/testclients

$CLIENTS/leftclient/leftclient &
$CLIENTS/rightclient/rightclient &
$CLIENTS/topclient/topclient &
$CLIENTS/bottomclient/bottomclient &
$CLIENTS/quickcenterclient/quickcenterclient &
$CLIENTS/widgetcenterclient/widgetcenterclient &
#EMBEDDED_SHELL_ANCHOR=center EMBEDDED_SHELL_SORT_INDEX=100 kcalc &

#notify-send 'test' 'test message'
#notify-send 'Attention 5' 'your fridge is running'
#notify-send 'Attention 3' 'your fridge is running'
#notify-send 'Attention 2' 'your fridge is running'
#notify-send 'Attention 6' 'your fridge is running'
#notify-send 'Attention 1' 'your fridge is running'

#dbus-monitor&

trap "echo signal handler; trap - TERM; kill $DBUS_SESSION_BUS_PID; pkill -P $$" INT TERM EXIT QUIT HUP PIPE
wait $compositor_pid
echo "compositor process exited."
