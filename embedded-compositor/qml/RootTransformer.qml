// SPDX-License-Identifier: GPL-3.0-only

import QtQuick
import QtWayland.Compositor
import QtQuick.Window
import QtQml.Models

import de.EmbeddedCompositor.dbus
import de.EmbeddedCompositor.embeddedshell

import "DefaultTaskSwitcher"
import "Notifications"

Item {
    id: rootTransformItem
    property bool fullScreen: false

    states: [
        State {
            name: "0"
            PropertyChanges {
                target: rootTransformItem
                rotation: 0
                width: parent.width
                height: parent.height
            }
        },
        State {
            name: "90"
            PropertyChanges {
                target: rootTransformItem
                rotation: 90
                width: parent.height
                height: parent.width
            }
        },
        State {
            name: "180"
            PropertyChanges {
                target: rootTransformItem
                rotation: 180
                width: parent.width
                height: parent.height
            }
        },
        State {
            name: "270"
            PropertyChanges {
                target: rootTransformItem
                rotation: 270
                width: parent.height
                height: parent.width
            }
        }
    ]

    transitions: [
        Transition{
            from:"*"
            to:"*"
            SequentialAnimation {
                RotationAnimation {
                    target: rootTransformItem
                    direction: RotationAnimation.Shortest
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
                PropertyAnimation {
                    target: rootTransformItem
                    properties: "width,height"
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
            }
        }

    ]

    width: parent.width
    height: parent.height
    anchors.centerIn: parent
}
