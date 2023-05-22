// SPDX-License-Identifier: GPL-3.0-only

import QtQuick 2.15
import QtWayland.Compositor 1.0
import QtQuick.Window 2.2
import QtQml.Models 2.1

import com.embeddedcompositor.dbus 1.0
import com.embeddedcompositor.embeddedshell 1.0
import com.embeddedcompositor.configuration 1.0

import "DefaultTaskSwitcher"
import "Notifications"

WaylandCompositor {
    WaylandOutput {
        sizeFollowsWindow: true
        window: Window {
            id: window
            width: configuration.screenWidth
            height: configuration.screenHeight
            visible: true
            property real initialSize: 0

            RootTransformer {
                id: rootTransformItem
                state: dbusScreenInterface.orientation

                Item {
                    Keys.onPressed:
                        (event)=> {
                            if (event.key === Qt.Key_Super_L || event.key === Qt.Key_Super_R) {
                                if(taskSwitcherLoader.item.state === "open") taskSwitcherLoader.item.close();
                                else taskSwitcherLoader.item.open();
                            } else if(event.key === Qt.Key_F1) {
                                rootTransformItem.state = "0"
                            } else if(event.key === Qt.Key_F2) {
                                rootTransformItem.state = "90"
                            } else if(event.key === Qt.Key_F3) {
                                rootTransformItem.state = "180"
                            } else if(event.key === Qt.Key_F4) {
                                rootTransformItem.state = "270"
                            } else {
                               screenSaverController.reset();
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
                    onSurfaceItemChanged: taskSwitcherInterface.currentView = surfaceItem.uuid
                    property ShellSurface surface
                    function selectSurface(shellSurface, view)
                    {
                        for(var i =0; i < children.length; i++)
                        {
                            var window = children[i];
                            if(window.shellSurface === shellSurface) {
                                window.currentView = view;
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
                    source: configuration.taskSwitcherUrl
                    active: true
                    onLoaded: {
                        item.surfaceModel = centerApplicationViewModel
                        item.switchTask.connect(doSwitch)
                    }

                    function doSwitch(shellSurface, view)
                    {
                        centerArea.selectSurface(shellSurface, view);
                    }
                }

                Notifications {
                    anchors.fill: parent
                }

                Loader {
                    id: globalOverlayLoader
                    anchors.fill:parent
                    source: configuration.globalOverlayUrl
                    active: true
                }

                ScreenSaverController {
                    id: screenSaverController
                    timeoutSeconds: configuration.screenSaverTimeoutSeconds
                    screenSaverUrl: configuration.screenSaverUrl
                }
            }
            Item {
                id: limboArea
                visible: false
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
                if (centerArea.surfaceItem.shellSurface === shellSurface)
                    centerArea.selectSurface(shellSurface, view);
                return;
            }

            append({data: {view: view, surface: shellSurface}});
        }

        function findByUuid(uuid)
        {
            for(var i = 0; i<count; i++)
            {
                var entry = get(i).data;
                if (entry.surface.uuid === uuid || entry.view !== null && entry.view.uuid === uuid)
                {
                    return entry
                }
            }
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
                handleAnchor();
            }

            property int margin: shellSurface.margin
            property string uuid: currentView ? currentView.uuid : shellSurface.uuid
            property var currentView: null

            onCurrentViewChanged: if(currentView != null) currentView.select();


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
                } else if(shellSurface.anchor === EmbeddedShellTypes.Undefined) {
                    shellSurfaceItem.parent = limboArea;
                    console.log("undefined -> noop");
                    return;
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
                if(width <= 0 || height <=0) return;
                shellSurface.sendConfigure(Qt.size(width, height));
            }
        }
    }


    EmbeddedShell {
        onSurfaceAdded: chromeComponent.createObject(limboArea, { "shellSurface": surface } );
    }
    TaskSwitcherInterface {
        id: taskSwitcherInterface
        onOpenRequested: taskSwitcherLoader.item.open();
        onCloseRequested: taskSwitcherLoader.item.close();
        viewModel: centerApplicationViewModel
        onCurrentViewChanged: {
            var entry = centerApplicationViewModel.findByUuid(currentView);
            centerArea.selectSurface(entry.surface, entry.view);
        }
    }
    GlobalOverlayInterface {
        onShowRequested: (message) => {
            globalOverlayLoader.item.show(message);
        }
        onHideRequested: globalOverlayLoader.item.hide()
    }

    CompositorScreenInterface {
        id: dbusScreenInterface
        screenSaverActive: screenSaverController.screenSaverActive
        onShowScreenSaver: screenSaverController.showScreenSaver();
    }

    ConfigurationHive {
        id: configuration
        property url taskSwitcherUrl: "DefaultTaskSwitcher/TaskSwitcher.qml"
        property url globalOverlayUrl: "DefaultGlobalOverlay/GlobalOverlay.qml"
        property url screenSaverUrl: "DefaultScreenSaver/ScreenSaver.qml"
        property int screenSaverTimeoutSeconds: 60
        property bool screenSaverMouseHoverSupport: false
        property int screenWidth: (Qt.application.primaryScreen || Qt.application.screens[0]).width
        property int screenHeight: (Qt.application.primaryScreen || Qt.application.screens[0]).height
    }
}
