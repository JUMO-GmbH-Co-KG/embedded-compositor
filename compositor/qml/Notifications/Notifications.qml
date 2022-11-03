import QtQuick 2.0
import com.embeddedcompositor.dbus 1.0
import com.embeddedcompositor.utility 1.0

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
            property var idx: model.index
            Component.onCompleted: {
                if(idx <= 0) {
                    state = "expanded"
                }
            }

            onIdxChanged: {
                if(idx <= 0) { // while being in removed animation it is -1
                    state = "expanded"
                } else {
                    state = "shade"
                }
            }
            state: "shade"

            width: ListView.view.width
            Text {
                color: "white"
                text:  idx +" "+parent.state
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
