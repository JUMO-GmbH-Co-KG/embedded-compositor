// SPDX-License-Identifier: GPL-3.0-only

import QtQuick 2.0
import de.EmbeddedCompositor.dbus 1.0

Rectangle {
    id: notificationRoot

    signal dismissed
    signal actionInvoked(string action)

    property string summary
    property string body
    property var actionNames
    property var actionLabels

    color: "#202040"
    Rectangle {
        id: header
        anchors {
            left: parent.left
            right:parent.right
            top:parent.top
            margins: 2
        }
        height: 32
        Text {
            color:"white"
            anchors.centerIn: parent
            font.pixelSize: 32
            text: notificationRoot.summary
        }
        color: "#404060"
    }

    Item {
        id: body
        anchors {
            top: header.bottom
            bottom: parent.bottom
            left: parent.left
            right:parent.right
            margins: 10
        }

        Text {
            font.pixelSize: 24
            text: notificationRoot.body
            color: "#d0d0d0"
            anchors {
                top: parent.top
                bottom:  buttons.top
                left:parent.left
                right: parent.right
            }
        }

        Row {
            id: buttons
            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }

            height:60
            width: parent.width
            spacing: 10
            Repeater {
                id: actionsRepeater
                model: notificationRoot.actionNames
                delegate: MouseArea {
                    width:200
                    height:parent.height
                    onPressed: {
                        notificationRoot.actionInvoked(modelData)
                    }

                    Rectangle {
                        anchors.fill: parent
                        color: parent.pressed ? "lightsteelblue" : "white"
                    }

                    Text {
                        anchors {
                            baseline: parent.verticalCenter
                            baselineOffset: font.pixelSize/2
                            horizontalCenter: parent.horizontalCenter
                        }
                        text: notificationRoot.actionLabels[index]
                        font.pixelSize:32
                    }
                }
            }
            MouseArea {
                visible: actionsRepeater.count === 0
                width:200
                height:parent.height
                onPressed: {
                    notificationRoot.dismissed()
                }

                Rectangle {
                    anchors.fill: parent
                    color: parent.pressed ? "lightsteelblue" : "white"
                }

                Text {
                    anchors {
                        baseline: parent.verticalCenter
                        baselineOffset: font.pixelSize/2
                        horizontalCenter: parent.horizontalCenter
                    }
                    text: "Ok"
                    font.pixelSize:32
                }
            }
        }
    }
}
