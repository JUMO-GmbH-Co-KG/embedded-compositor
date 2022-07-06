import QtQuick 2.0
import QtWayland.Compositor 1.0
import QtQuick.Window 2.2

import com.embeddedcompositor.embeddedshell 1.0

WaylandCompositor {
    WaylandOutput {
        sizeFollowsWindow: true
        window: Window {
            id: window
            width: 800
            height: 480
            visible: true
            property real initialSize: 0
            Rectangle {
                id: centerArea
                anchors.top: topArea.bottom
                anchors.left: leftArea.right
                anchors.right: rightArea.left
                anchors.bottom: bottomArea.top
                border {
                    width:4
                    color:"red"
                }
                property Item surfaceItem
            }
            Rectangle {
                id:leftArea
                width: surfaceItem ? surfaceItem.initialWidth :window.initialSize
                anchors.bottom: bottomArea.top
                anchors.left: parent.left
                anchors.top: topArea.bottom
                border {
                    width:1
                    color:"green"
                }
                color: "darkgreen"
                property Item surfaceItem
            }
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
            Rectangle {
                id: topArea
                height: surfaceItem ? surfaceItem.initialHeight : window.initialSize
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                border {
                    width: 1
                    color:"green"
                }
                color: "darkgreen"
                property Item surfaceItem
            }
            Rectangle {
                id: bottomArea
                height: surfaceItem ? surfaceItem.initialHeight : window.initialSize
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                border {
                    width:1
                    color:"green"
                }
                color: "darkgreen"
                property Item surfaceItem
            }
        }
    }
    Component {
        id: chromeComponent
        ShellSurfaceItem {
            id: shellSurfaceItem
            onSurfaceDestroyed: {
                destroy()
            }
            onWidthChanged: handleResized()
            onHeightChanged: handleResized()
            property int initialHeight: -1
            property int initialWidth: -1

            function handleResized() {
                console.log(shellSurface.iviId+": "+width+"x"+height);
                if(width < 0 || height <0) return;
                if(initialWidth < 0) initialWidth = width;
                if(initialHeight < 0) initialHeight = height;
                shellSurface.sendConfigure(Qt.size(width, height));
            }
        }
    }

    EmbeddedShell {
        Component.onCompleted: console.log("INITIALIZED!");
        onSurfaceAdded: {
            console.log("QML: shell surface created: "+surface+" "+surface.anchor);
        }
    }
}
