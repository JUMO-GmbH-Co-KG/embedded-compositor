import QtQuick 2.0

Rectangle {
    id:rightArea
    width: surfaceItem ? surfaceItem.initialWidth :window.initialSize
    anchors.bottom:bottomArea.top
    anchors.right: parent.right
    anchors.top: topArea.bottom
    border {
        width:1
        color:"green"
    }
    color: "darkgreen"
    property Item surfaceItem
}
