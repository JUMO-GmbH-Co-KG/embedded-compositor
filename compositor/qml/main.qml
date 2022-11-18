import QtQuick 2.0
import QtWayland.Compositor 1.0
import QtQuick.Window 2.2
import QtQml.Models 2.1

import com.embeddedcompositor.dbus 1.0
import com.embeddedcompositor.embeddedshell 1.0
import "DefaultTaskSwitcher"
import "Notifications"

WaylandCompositor {
    WaylandOutput {
        sizeFollowsWindow: true
        window: Window {
            id: window
            width: 1280
            height: 800
            visible: true
            property real initialSize: 0

            Item {
                Keys.onPressed:
                    (event)=> {
                        if (event.key == Qt.Key_Super_L || event.key == Qt.Key_Super_R) {
                            if(taskSwitcherLoader.item.state == "open") taskSwitcherLoader.item.close();
                            else taskSwitcherLoader.item.open();
                        }
                }
                focus: true
            }

            Rectangle {
                id: centerArea
                anchors.top: topArea.bottom
                anchors.left: leftArea.right
                anchors.right: rightArea.left
                anchors.bottom: bottomArea.top
                border {
                    width: 1
                    color:"red"
                }
                property Item surfaceItem
                property ShellSurface surface
                function selectSurface(shellSurface, view)
                {
                    for(var i =0; i < children.length; i++)
                    {
                        var window = children[i];
                        if(window.shellSurface === shellSurface) {
                            surfaceItem = window;
                            break;
                        }
                    }
                }
            }
            Item {
                id:leftArea
                width: surfaceItem ? surfaceItem.margin:window.initialSize
                anchors.bottom: bottomArea.top
                anchors.left: parent.left
                anchors.top: topArea.bottom
                property Item surfaceItem
            }
            Item {
                id:rightArea
                width: surfaceItem ? surfaceItem.margin :window.initialSize
                anchors.bottom:bottomArea.top
                anchors.right: parent.right
                anchors.top: topArea.bottom
                property Item surfaceItem
            }
            Item {
                id: topArea
                height: surfaceItem ? surfaceItem.margin : window.initialSize
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                property Item surfaceItem
            }
            Item {
                id: bottomArea
                height: surfaceItem ? surfaceItem.margin : window.initialSize
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                property Item surfaceItem
            }

            Loader {
                id:taskSwitcherLoader
                anchors.fill:parent
                // this path probably only works while debugging on desktop
                // source: "file:../alt-switcher/GridSwitcher.qml"
                source:"DefaultTaskSwitcher/TaskSwitcher.qml"
                active: true
                onLoaded: {
                    item.surfaceModel = centerApplicationViewModel
                    item.switchTask.connect(doSwitch)
                }
                function doSwitch(shellSurface, view)
                {
                    if(view !== null) view.select();
                    centerArea.selectSurface(shellSurface, view);
                }
            }

            Notifications {
                anchors.fill: parent
            }

            GlobalOverlay {
                id: globalOverlay
            }
        }
    }

    property var anchorMap: ({
         [EmbeddedShellTypes.Top]: topArea,
         [EmbeddedShellTypes.Bottom]: bottomArea,
         [EmbeddedShellTypes.Left]: leftArea,
         [EmbeddedShellTypes.Right]: rightArea,
         [EmbeddedShellTypes.Center]: centerArea,
         [EmbeddedShellTypes.Undefined]: limboArea,
    })

    ListModel {
        id: centerApplicationViewModel
        property var surfaces: ({});

        function addSurface(shellSurface)
        {
            surfaces[shellSurface] = {
                views: []
            };
            append({data: {view: null, surface: shellSurface}});
        }

        function removeSurface(shellSurface)
        {
            for(var i = count - 1; i>0; i--)
            {
                if(get(i).data.surface === shellSurface)
                {
                    remove(i);
                }
            }
            delete surfaces[shellSurface]
        }

        function createView(shellSurface, view)
        {
            var entry = surfaces[shellSurface];
            console.log("compositor: create view! "+ view +" have entry "+entry);
            if(entry.views.length === 0)
            {
                console.log("setting first view!");
                entry.views.push(view);
                append({data: {view: view, surface: shellSurface}});
                for(var i = 0; i<count; i++)
                {
                    if(get(i).data.surface == shellSurface)
                    {
                        remove(i);
                        break;
                    }
                }
                return;
            }

            append({data: {view: view, surface: shellSurface}});
        }
    }

    Component {
        id: chromeComponent
        ShellSurfaceItem {
            id: shellSurfaceItem

            visible: parent.surfaceItem === shellSurfaceItem

            onSurfaceDestroyed:  destroy()
            onWidthChanged: handleResized()
            onHeightChanged: handleResized()
            Component.onCompleted: {
                handleResized();
                handleAnchor();
            }

            property int margin: shellSurface.margin

            Connections {
                target: shellSurface
                function onAnchorChanged() {
                    handleAnchor();
                }

                function onCreateView(view) {
                    centerApplicationViewModel.createView(shellSurface, view);
                }
            }

            function handleAnchor() {
                var targetArea = anchorMap[shellSurface.anchor];

                if(shellSurface.anchor === EmbeddedShellTypes.Center) {
                    centerApplicationViewModel.addSurface(shellSurface, shellSurfaceItem);
                }

                shellSurfaceItem.parent = targetArea;
                shellSurfaceItem.anchors.left = targetArea !== rightArea ? targetArea.left : undefined;
                shellSurfaceItem.anchors.right = targetArea !== leftArea ? targetArea.right : undefined;
                shellSurfaceItem.anchors.top = targetArea !== bottomArea ? targetArea.top : undefined;
                shellSurfaceItem.anchors.bottom = targetArea !== topArea ? targetArea.bottom : undefined;

                if(shellSurface.anchor !== EmbeddedShellTypes.Undefined) {
                    targetArea.surfaceItem = shellSurfaceItem;
                }
            }

            function handleResized() {
                console.log("handleResize anchor:"+shellSurface.anchor+" size "+width+"x"+height);
                if(width < 0 || height <0) return;
                shellSurface.sendConfigure(Qt.size(width, height));
            }
        }
    }

    Item {
        id: limboArea
        visible: false
    }

    EmbeddedShell {
        onSurfaceAdded: chromeComponent.createObject(limboArea, { "shellSurface": surface } );
    }
    TaskSwitcherInterface {
        onOpenRequested: taskSwitcherLoader.item.open();
        onCloseRequested: taskSwitcherLoader.item.close();
    }
    GlobalOverlayInterface {
        onShowRequested: globalOverlay.show(message)
        onHideRequested: globalOverlay.hide()
    }
}
