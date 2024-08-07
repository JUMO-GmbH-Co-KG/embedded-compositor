// SPDX-License-Identifier: GPL-3.0-only

import QtQuick 2.0
import de.EmbeddedCompositor.dbus 1.0

Rectangle {
    id: notificationRoot

    signal dismissed
    signal actionInvoked(string action)

    property string summary
    property string body
    property string appIcon
    property var actionNames
    property var actionLabels
    property bool actionIcons

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

        Image {
            id: appIcon
            anchors.centerIn: parent
            source: notificationRoot.appIcon
            sourceSize.width: 64
            sourceSize.height: 64
        }

        Item{
            anchors {
                margins: 10
                top: appIcon.bottom
                bottom:  buttons.top
                left: parent.left
                right: parent.right
            }
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 24
                text: notificationRoot.body
                color: "#d0d0d0"
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
                    width:parent.height
                    height:parent.height
                    onPressed: {
                        notificationRoot.actionInvoked(modelData)
                    }

                    Rectangle {
                        anchors.fill: parent
                        color: parent.pressed ? "lightsteelblue" : "white"
                        visible: !notificationRoot.actionIcons
                    }

                    Text {
                        anchors {
                            centerIn: parent
                        }
                        text: notificationRoot.actionLabels[index]
                        font.pixelSize:32
                        visible: !notificationRoot.actionIcons
                    }

                    Image {
                        id: icon
                        anchors.fill: parent
                        source: notificationRoot.actionIcons ? notificationRoot.actionLabels[index] : ""
                        sourceSize.width: height
                        sourceSize.height: height
                        visible: notificationRoot.actionIcons
                    }
                }
            }
        }
    }
}
