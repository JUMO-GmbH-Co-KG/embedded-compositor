// SPDX-License-Identifier: LGPL-3.0-only

import QtQuick 2.15
import EmbeddedShell 1.0 as EmbeddedShell

EmbeddedShell.Window {
    id: window
    height: 48
    visible: true
    title: qsTr("Hello World")
    color: "grey"
    anchor: EmbeddedShell.Window.Anchor.Bottom
    margin: 48

    Row {
        anchors {
            fill:parent
            margins: 4
        }
        spacing:10

        Rectangle {
            width: 100
            height: 40
            color: "plum"
            MouseArea {
                anchors.fill:parent
                onClicked: {
                    dbusClient.openTaskSwitcher();
                }
            }
            Text {
                anchors.centerIn: parent
                text: "⊞"
                font.pixelSize: 48
            }
        }

        Rectangle {
            width: 100
            height: 40
            color: "plum"
            MouseArea {
                anchors.fill:parent
                onClicked: {
                    dbusClient.showGlobalOverlay("reticulating splines, please be patient.");
                    overlayTimer.start();
                }
                Timer {
                    id:overlayTimer
                    interval: 3500
                    repeat: false
                    onTriggered: dbusClient.hideGlobalOverlay();
                }
            }

            Text {
                anchors.centerIn: parent
                text: "⚠️"
                font.pixelSize: 48
            }
        }

        Rectangle {
            id: notificationTestButton

            width: 100
            height: 40
            color: "tomato"
            property int dialogId: -1
            MouseArea {
                anchors.fill:parent
                onClicked: {
                    parent.dialogId = dbusClient.notify("Choose color", "Please choose the color for the bottom window", ["grey", "plum", "lime"])
                }
            }
            Text {
                anchors.centerIn: parent
                text: "⊞" + (parent.dialogId !== -1 ? (" "+parent.dialogId): "")
                font.pixelSize: 48
            }
        }
        Text {
            width:200
            id: responseLog
            text:"..."
            Component.onCompleted: {
                dbusClient.notificationActionInvoked.connect(notificationActionInvoked )
            }

            function notificationActionInvoked (id, action) {
                text = id + "->" +action + "\n"+text
                if(id === notificationTestButton.dialogId) {
                    window.color = action;
                    dbusClient.notify("color change complete.", "thank you.", ["OK"])
                }
            }

        }

        Rectangle {
            width: 40
            height: 40
            color: "plum"
            MouseArea {
                anchors.fill:parent
                onClicked:  dbusClient.setOrientation("0")
            }

            Text {
                anchors.centerIn: parent
                text: "0"
                font.pixelSize: 24
            }
        }
        Rectangle {
            width: 40
            height: 40
            color: "plum"
            MouseArea {
                anchors.fill:parent
                onClicked:  dbusClient.setOrientation("90")
            }

            Text {
                anchors.centerIn: parent
                text: "90"
                font.pixelSize: 24
            }
        }
        Rectangle {
            width: 40
            height: 40
            color: "plum"
            MouseArea {
                anchors.fill:parent
                onClicked:  dbusClient.setOrientation("180")
            }

            Text {
                anchors.centerIn: parent
                text: "180"
                font.pixelSize: 24
            }
        }
        Rectangle {
            width: 40
            height: 40
            color: "plum"
            MouseArea {
                anchors.fill:parent
                onClicked:  dbusClient.setOrientation("270")
            }

            Text {
                anchors.centerIn: parent
                text: "270"
                font.pixelSize: 24
            }
        }
    }

    Component.onCompleted: dbusClient.hideGlobalOverlay()
}
