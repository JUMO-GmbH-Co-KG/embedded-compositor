import QtQuick 2.15
import QtWayland.Compositor 1.0
import QtQuick.Window 2.2
import QtQml.Models 2.1

import de.EmbeddedCompositor.dbus 1.0
import de.EmbeddedCompositor.embeddedshell 1.0
import de.EmbeddedCompositor.configuration 1.0

import "DefaultTaskSwitcher"
import "Notifications"

Item {
    id: screenSaverController
    anchors.fill: parent
    property int timeoutSeconds: 30
    property url screenSaverUrl
    readonly property bool screenSaverActive: screenSaverLoader.active
    property bool mouseHoverSupport
    property bool screenSaverEnabled: true

    function reset()
    {
        screenSaverLoader.active = false;
        if(screenSaverEnabled) {
            screenSaverTimeOut.restart();
        }
    }

    onScreenSaverEnabledChanged: {
        if(!screenSaverEnabled) {
            screenSaverLoader.active = false;
        }
    }

    function showScreenSaver()
    {
        screenSaverLoader.active = true;
    }

    Timer {
        id: screenSaverTimeOut
        running: screenSaverEnabled
        interval: screenSaverController.timeoutSeconds * 1000
        onTriggered: {
            screenSaverLoader.active = true;
        }
    }

    Loader {
        id: screenSaverLoader
        anchors.fill:parent
        source: screenSaverUrl
        active: false
    }

    Item {
        anchors.fill: parent

        // on platforms with mouse only
        HoverHandler
        {
            id: hoverHandler
            enabled: screenSaverController.mouseHoverSupport
            property int xp: point.position.x
            property int yp: point.position.y
            onXpChanged: screenSaverController.reset();
            onYpChanged: screenSaverController.reset();
        }

        PointHandler {
            id: handler
            onPointChanged: screenSaverController.reset();
        }
        Keys.onPressed: screenSaverController.reset();
    }

    MouseArea {
        anchors.fill: parent
        enabled: screenSaverLoader.active
        onClicked: screenSaverController.reset();
    }
}
