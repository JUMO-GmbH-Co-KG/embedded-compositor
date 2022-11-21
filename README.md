# Embedded-compositor

This project implements a wayland-based compositor suited for industrial HMIs. The implementation makes
use of the [Qt Wayland module](https://doc.qt.io/qt-5/qtwaylandcompositor-index.html).

The compositor implements window management through its QML UI.
It is based around a custom wayland shell interface that allows windows to specify

* an anchor representing a screen edge or the center area, and
* a margin representing the width in pixels the window will reserve from that edge (if not center).

After the reserved amount of screen space is subtracted from each edge, the remaining
screen area is provided to applications displayed in the center area.

Wayland clients occupying one of the edge slots are expected to represent system launchers or menus,
while the center area is expected to house the mein application content windows.

Edge occupying clients can resize themselves bigger than the originally reserved size, however this will not change the dimensions of other clients.
This is expected to be used to allow opening and closing of menus that overlay the central application.

## Virtual Surface Views

We expect some applications having multiple views that should be listed in a task switcher independently, while belonging to the same logical window/wayland surface.
For this purpose we devised an interface called "surface_view" that represents a view within the applications logic that can be switched to via tha global task switch UI.

## Sorting Windows and Views

It can be desirable to provide a predictable order of surfaces in the task switcher and for this purpose we added an index that surfaces and surface views can be sorted by.
It is expected that clients read their sort index from configuration provided by the system integrator.

## Wayland Protocol Extension

The Anchor/Margin functionality is provided by way of an extension we call "embedded_shell".
The main interface is the embedded_shell_surface and it is expected that clients bind to it to be shown in our compositor. For Qt applications we provide a plugin that handles this integration via the QPA.
The embedded_shell_surface provides a request to additionally create objects bound to the surface_view interface. These objects are not surfaces but just handles to facilitate switching to a virtual view of a embedded_shell_surface.

## Architecture

### Client side

We developed with Qt clients in mind which will have wayland integration available automatically by way of the Qt wayland QPA.
The wayland QPA is a plugin loaded by Qt plugins when `QT_QPA_PLATFORM=wayland` is set.
The wayland QPA itself again has a plugin mechanism allowing it to load a different shell integration.
We implement this plugin interface in the subproject "shellintegration" to integrate our embedded_shell into Qt clients.

When clients create their window, the QPA will load the embedded_shell plugin when `QT_WAYLAND_SHELL_INTEGRATION=embedded-shell` is set and bind to our shell interface. Through this alone, however clients have no access to our custom properties.
We provide an additional library called embeddedplatform that provides access to our custom interfaces and properties to client application code.
By linking this library, clients can acquire a pointer to a `EmbeddedShellSurface` (see [.h](embeddedplatform/embeddedshellsurface.h), [.cpp](embeddedplatform/embeddedshellsurface.cpp)) instance that allows access to shell surface properties and methods to create views. 

```c++
class MainWindow : public QMainWindow {
  Q_OBJECT
  QLabel *m_label = nullptr;

public:
  MainWindow(QWidget *parent = nullptr) {
    m_label = new QLabel("initial state", this);
    this->setCentralWidget(m_label);
    // callback when the QPA has finished creating the shell integration,
    // at which point we can start using the API with the provided EmbeddedShellSurface object
    connect(EmbeddedPlatform::instance(), &EmbeddedPlatform::shellSurfaceCreated, this, &MainWindow::initShell);
  }
  ~MainWindow() override {}

private slots:
  void initShell(EmbeddedShellSurface *shellSurface, QWindow *window) {
    // the callback is invoked for all windows but in this
    // example we will only look for this one
    if (this->windowHandle() != window) {
        return;
    }
    if (shellSurface == nullptr) {
        return;
    }
    shellSurface->sendAnchor(EmbeddedShellTypes::Anchor::Center);
    auto v1 = shellSurface->createView("View One", 3);
    auto v2 = shellSurface->createView("View Two", 2);
    auto v3 = shellSurface->createView("View Three", 1);
    connect(v1, &EmbeddedShellSurfaceView::selected, this,
            [=] { m_label->setText(v1->label()); });
    connect(v2, &EmbeddedShellSurfaceView::selected, this,
            [=] { m_label->setText(v2->label()); });
    connect(v3, &EmbeddedShellSurfaceView::selected, this,
            [=] { m_label->setText(v3->label()); });
  }
```

There is an alternative method to detect shell surface creation which is waiting for a certain window event after which we found the shell surface to exist:

```c++
    bool MainWindow::event(QEvent *event) {
      // found this event happens just after shell surface creation
      if (event->type() == QEvent::ShowToParent)
        initShell();
      return QMainWindow::event(event);
    }

```

For convenience of QML applications, we also implement a QML interface to embedded_shell_surface, which is implemented in the subproject quickembeddedshellwindow

```qml
    import EmbeddedShell 1.0
    Window {
        id: window
        visible: true
        title: qsTr("Hello from CenterCLient")
        anchor: Window.Anchor.Center
        color: "darkgray"
        width: 200
        height:200
        MouseArea {
            anchors.fill: parent
            onClicked: {
                var view = window.createView("view name", 42);
                view.selected.connect(function(){ console.log("view "+ view.label +" was selected"); })
            }
    }
```

For third party Qt applications that can not be adapted to interface with the `embedded-shell` protocol, we provide a set of environment variables to choose a compositor slot, width and sort index:

```sh
    # possible values: center, top, bottom, left, right
    export EMBEDDED_SHELL_ANCHOR=center
    # integer values
    export EMBEDDED_SHELL_SORT_INDEX=100
    # unsigned integer values
    export EMBEDDED_SHELL_MARGIN=32
    # sample application
    kcalc
```

### Compositor side

The compositor is built around Qt's qtwayland qml compositor examples with a custom shell implemented similar to the IVI shell example.
We leverage `QWaylandShellTemplate<EmbeddedShellExtension>` and `public QWaylandShellSurfaceTemplate<EmbeddedShellSurface>` to register our shell extension and surface interface with Qt.

We require surfaces to bind to `embedded_shell` and to specify their anchor edge before the compositor displays them. Unfortunately it is tricky to get these values from clients from the moment of creating the `embedded_shell_surface` interface, so until the anchor changes from `undefined` a surface remains hidden.

## Implementation Notes

It has proven tricky to provide our custom surface properties anchor, margin or sort_index early enough to be able to provide them with the initial binding of the embedded_shell_surface to the plain wl_surface through Qt's wayland integration.
Therefore we need to rely on setting these properties after the interface has been bound with initial undefined or zero values through the set_* requests also defined in the protocol.

We provide logging through [Qt's logging framework](https://doc.qt.io/qt-5/qloggingcategory.html) and define logging categories which can be enabled as follows.

```sh
    export QT_LOGGING_RULES="embeddedshell.compositor.debug=true;embeddedshell.quick=true"
```

## DBus integration

The compositor implements a couple of DBus interfaces to provide system/window manager services:

* com.embeddedcompositor.taskswitcher - allows the task switcher to be opened or closed from e.g. a menu button.
* com.embeddedcompositor.globaloverlay - displays a global overlay hiding all other UI elements for a boot or shutdown message.
* org.freedesktop.Notifications - implements a small subset of notifications which are displayed as a global modal until dismissed or one of the options is chosen.

## Building

Building is straight forward through qmake and make or Qt creator. The egnerated makefiles should allow you to make install the project on a target device.

```sh
~/src/compositor $ mkdir ../build-compositor
~/src/compositor $ cd ../build-compositor
~/src/build-compositor $ qmake ../compositor
~/src/build-compositor $ make
```

Dependencies are Qt 5.12, configured with DBus, wayland and QtQuick.

The project can be developed and tested on a Linux workstation and the provided run.sh file tries to start the compositor and some test clients as well as an isolated DBus instance as to not confuse the hosts system.

The project was tested against an IMX platform where the unstable qt dma buf protocol is required and needs to be set in the environment:

```sh
export QT_WAYLAND_CLIENT_BUFFER_INTEGRATION=linux-dmabuf-unstable-v1
export QT_QPA_PLATFORM=eglfs
```

## test clients

we provide a set of test clients to populate the areas available in the compositor layout:

* top-, left- and rightclient: qml apps that attach to the top, left and right edge
* bottomclient: qml app that occupies the bottom spot, provides buttons for testing the DBus interfaces for accessing the taskswitcher, opening the global overlay and opening notifications and reacting to the action chosen by the user.
* quickcenterclient - qml application that creates some views after each click of a mousearea
* widgetcenterclient - plain Qt c++ application that creates a set of views

## Licensing

The wayland compositor is licensed under the GPLv3.
The shellintegration is licensed under the LGPLv3.
