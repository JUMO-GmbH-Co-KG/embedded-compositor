// SPDX-License-Identifier: GPL-3.0-only

import QtQuick
import QtWayland.Compositor
import QtQuick.Window
import QtQml.Models

import de.EmbeddedCompositor.dbus
import de.EmbeddedCompositor.embeddedshell
import de.EmbeddedCompositor.configuration

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
