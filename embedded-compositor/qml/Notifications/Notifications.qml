// SPDX-License-Identifier: GPL-3.0-only

import QtQuick
import QtQuick.Controls

import de.EmbeddedCompositor.dbus
import de.EmbeddedCompositor.utility

Rectangle {
    color: Qt.rgba(0,0,0,0.5)
    visible: notifications.count > 0

    NotificationModel {
        id: notifications
    }

    StackView {
        id: stack
        anchors.fill: parent
        anchors.margins: 10
        Repeater {
            model: notifications
            delegate: Loader{
                id: notification
                anchors.fill: parent

                source: configuration.notificationUrl

                onLoaded: {
                    if(item) {
                        item.summary = summary
                        item.body = body
                        item.appIcon = appIcon
                        item.actionNames = actionNames
                        item.actionLabels = actionLabels
                        item.actionIcons = actionIcons

                        item.onDismissed.connect(function() {
                            const idx = notifications.index(index, 0);
                            notifications.dismiss(idx);
                        });
                        item.actionInvoked.connect(function(action) {
                            const idx = notifications.index(index, 0);
                            notifications.invokeAction(idx, action);
                        });
                    }
                }
            }
        }
    }
}

