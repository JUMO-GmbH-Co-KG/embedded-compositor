// SPDX-License-Identifier: GPL-3.0-only

import QtQuick 2.0
import de.EmbeddedCompositor.dbus 1.0
import de.EmbeddedCompositor.utility 1.0

Rectangle {
    color: Qt.rgba(0,0,0,0.5)
    visible: notifications.count > 0

    MouseArea {
        anchors.fill: parent
        onClicked: {}
    }

    NotificationModel {
        id: notifications
    }

    ListView {
        id: list
        interactive: false
        anchors.fill: parent
        anchors.margins: 100
        spacing: 1
        model: notifications
        delegate: Notification {
            required property int index

            state: index <= 0 ? "expanded" : "shade"

            onDismissed: {
                const idx = notifications.index(index, 0);
                notifications.dismiss(idx);
            }
            onActionInvoked: (action) => {
                const idx = notifications.index(index, 0);
                notifications.invokeAction(idx, action);
            }

            width: ListView.view.width
            Text {
                color: "white"
                text:  index +" "+parent.state
            }
        }

        remove: Transition {
            ParallelAnimation {
                PropertyAnimation { property: "x"; to: 200; duration: 500; easing.type: Easing.OutQuad }
                PropertyAnimation { property: "opacity"; to: 0; duration: 500; easing.type: Easing.OutQuad }
            }
        }

        displaced: Transition {
            SequentialAnimation {
                PauseAnimation{
                    duration: 500
                }

                NumberAnimation { properties: "x,y"; duration: 250; easing.type: Easing.InOutQuad }
            }
        }

        add: Transition {
            ParallelAnimation {
                PropertyAnimation { property: "x"; from: -200; to: 0; duration: 500; easing.type: Easing.OutQuad }
                PropertyAnimation { property: "opacity"; from: 0; to: 1; duration: 500; easing.type: Easing.OutQuad }
            }
        }
    }
}
