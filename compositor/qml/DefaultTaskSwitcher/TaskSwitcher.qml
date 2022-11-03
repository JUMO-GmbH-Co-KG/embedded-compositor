import QtQuick 2.0
import QtWayland.Compositor 1.0
import com.embeddedcompositor.embeddedshell 1.0
import QtGraphicalEffects 1.12
import com.embeddedcompositor.utility 1.0

Rectangle {
    id: taskSwitcher
    color: Qt.rgba(0.0,0.0,0.0,0.8)
    anchors.fill: parent

    signal switchTask(shellSurface: ShellSurface, view: SurfaceView);

    property var surfaces: ({});
    function addSurface(shellSurface)
    {
        surfaces[shellSurface] = {
            views: []
        };
        surfaceModel.append({data: {view: null, surface: shellSurface}});
    }

    function removeSurface(surface)
    {
        for(var i = surfaceModel.count -1; i>0; i--)
        {
            if(surfaceModel.get(i).data.surface == shellSurface)
            {
                surfaceModel.remove(i);
            }
        }
    }

    function createView(shellSurface, view)
    {
        var entry = surfaces[shellSurface];
        console.log("compositor: create view! "+ view +" have entry "+entry);
        if(entry.views.length === 0)
        {
            console.log("setting first view!");
            entry.views.push(view);
            surfaceModel.append({data: {view: view, surface: shellSurface}});
            for(var i = 0; i<surfaceModel.count; i++)
            {
                if(surfaceModel.get(i).data.surface == shellSurface)
                {
                    surfaceModel.remove(i);
                    break;
                }
            }
            return;
        }

        surfaceModel.append({data: {view: view, surface: shellSurface}});
    }

    function open()
    {
        state = "open"
    }

    function close()
    {
        state = "closed"
    }


    ListModel {
        id: surfaceModel
    }

    SortFilterProxyModel {
        id: sorted
        sourceModel: surfaceModel
        sortFunction: "sort"
        function sort(a,b) {
            var ai = a.view ? a.view.sortIndex : a.surface.sortIndex
            var bi = b.view ? b.view.sortIndex : b.surface.sortIndex
            return ai<bi;
        }
    }

    ListView {
        z:100
        model: sorted

        anchors.centerIn: parent
        width: contentWidth < parent.width ? contentWidth : parent.width
        height: contentHeight

        delegate: taskSwitchComponent
        spacing: 5
        orientation: ListView.Horizontal
    }

    MouseArea {
        anchors.fill: parent
        onClicked: taskSwitcher.close()
    }

    Component {
        id: taskSwitchComponent
        Rectangle {
            id: taskSwitchComponentRoot
            color: "lime"
            property alias shellSurface: sItem.shellSurface
            property SurfaceView view: model.data.view
            property alias locked: sItem.bufferLocked
            width: 1280 / 4
            height: 800 / 4

            ShellSurfaceItem {
                anchors.centerIn: parent
                scale: parent.width / sItem.width
                id: sItem
                visible: taskSwitcher.visible
                shellSurface: model.data.surface
            }

            FastBlur {
                anchors.fill:parent
                source: sItem
                radius: 64
                cached: taskSwitchComponentRoot.locked
            }

            Text {
                id: text
                anchors.centerIn: parent
                text: (view? ("<"+view.label+"> "+view.sortIndex) : "NOT VIEW "+shellSurface.sortIndex)
                color:"white"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log("switch task",shellSurface, view);
                    var entry = taskSwitcher.surfaces[shellSurface];
                    for(var i = 0; i < entry.items.length; i++)
                    {
                        var item = entry.items[i];
                        item.locked = (item !== taskSwitchComponentRoot);
                    }

                    taskSwitcher.switchTask(shellSurface, view);
                    taskSwitcher.close()
                }
            }
        }
    }

    state: "closed"
    states: [
        State {
            name: "open"
            PropertyChanges {
                target: taskSwitcher
                visible: true
                opacity: 1
            }
        },
        State {
            name: "closed"
            PropertyChanges {
                target: taskSwitcher
                visible: false
                opacity: 0
            }
        }

    ]

    transitions: [
        Transition {
            from:"closed"
            to:"open"
            reversible: true

            SequentialAnimation {
                PropertyAction { property: "visible" }
                PropertyAnimation {
                    property: "opacity"
                    duration: 200
                }
            }
        }
    ]
}
