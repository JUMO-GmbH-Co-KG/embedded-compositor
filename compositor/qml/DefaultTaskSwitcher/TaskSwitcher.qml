import QtQuick 2.0
import QtWayland.Compositor 1.0
import com.embeddedcompositor.embeddedshell 1.0
import QtGraphicalEffects 1.12

Rectangle {
    id: taskSwitcher
    color: Qt.rgba(0.0,0.0,0.0,0.8)
    anchors.fill: parent

    signal switchTask(shellSurface: ShellSurface, view: SurfaceView);

    property var surfaces: ({});
    function addSurface(shellSurface)
    {
        var taskSwitchItem = taskSwitchComponent.createObject(
                    taskSwitchContainer, {
                        "shellSurface": shellSurface,
                        "view": null
                    });

        surfaces[shellSurface] = {
            items: [taskSwitchItem],
            currentItem: taskSwitchItem
        };
    }

    function removeSurface(surface)
    {

    }

    function createView(shellSurface, view)
    {
        var entry = surfaces[shellSurface];
        console.log("compositor: create view! "+ view +" have entry "+entry);
        if(entry.items[0].view == null)
        {
            console.log("setting first view!");
            entry.items[0].view = view;
            return;
        }

        var viewItem = taskSwitchComponent.createObject(taskSwitchContainer, {
                                                      view: view,
                                                      shellSurface: shellSurface,
                                                  });
        entry.items.push(viewItem);
    }

    function open()
    {
        state = "open"
    }

    function close()
    {
        state = "closed"
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

    MouseArea {
        anchors.fill: parent
        onClicked: taskSwitcher.close()
    }

    Flickable {
        id: taskSwitcherFlick
        anchors.centerIn: parent
        width: contentWidth < parent.width ? contentWidth : parent.width
        height: contentHeight
        flickableDirection: Flickable.HorizontalFlick
        contentHeight: taskSwitchContainer.height
        contentWidth: taskSwitchContainer.width
        Row {
            id: taskSwitchContainer
            height: 400
            spacing: 10
        }
    }

    Component {
        id: taskSwitchComponent
        Rectangle {
            id: taskSwitchComponentRoot
            color: "lime"
            property alias shellSurface: sItem.shellSurface
            property SurfaceView view: null
            property alias locked: sItem.bufferLocked
            width: 1280 / 4
            height: 800 / 4
            ShellSurfaceItem {
                anchors.centerIn: parent
                scale: parent.width / sItem.width
                id: sItem
                onSurfaceDestroyed: {
                    taskSwitchComponentRoot.destroy()
                }
                visible: taskSwitcher.visible
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
                text: view? ("<"+view.label+">") : "NOT VIEW"
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
}
