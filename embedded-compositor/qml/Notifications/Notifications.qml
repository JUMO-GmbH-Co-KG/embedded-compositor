// SPDX-License-Identifier: GPL-3.0-only

import QtQuick 2.0
import QtQuick.Layouts 1.15
import de.EmbeddedCompositor.dbus 1.0
import de.EmbeddedCompositor.utility 1.0

Rectangle {
    color: Qt.rgba(0,0,0,0.5)
    visible: notifications.count > 0

    NotificationModel {
        id: notifications
    }

    StackLayout {
        id: list
        anchors.fill: parent
        anchors.margins: 10
        Repeater {
            model: notifications
            delegate: Notification {
                required property int index

                onDismissed: {
                    const idx = notifications.index(index, 0);
                    notifications.dismiss(idx);
                }
                onActionInvoked: (action) => {
                    const idx = notifications.index(index, 0);
                    notifications.invokeAction(idx, action);
                }
            }
        }
    }
}
