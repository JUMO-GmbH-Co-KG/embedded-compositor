import QtQuick 2.0
import com.embeddedcompositor.dbus 1.0

Rectangle {
    color: Qt.rgba(0,0,0,0.5)
    visible: notifications.count > 0

    NotificationModel {
        id: notifications
    }
    ListView {
        id: list
        interactive: false
        anchors.fill: parent
        anchors.margins: 50
        spacing: 1
        model: notifications
        delegate: Notification {
            property var idx: model.index
            onIdxChanged: {
                console.log(idx);
                if(idx === 0) {
                    state = "expanded"
                }
            }
            state: "shade"

            width: ListView.view.width
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
