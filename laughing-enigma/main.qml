import QtQuick 2.6
import QtQuick.Controls 2.3
import QtQuick.Dialogs 1.0
import QtQuick.Layouts 1.3

import QtQuick.Controls.Material 2.0

ApplicationWindow {
    id: root
    objectName: "root"
    width: 900
    height: 600
    visible: true

    Material.theme: Material.Light
    Material.primary: Material.BlueGray
    Material.accent: Material.Teal

    signal openBookSignal(string msg)

    FileDialog {
        id: bookFileDialog
        objectName: "bookFileDialog"
        title: "Please choose a file"
        visible: false
        nameFilters: ["*.fb2"]
        onAccepted: openBookSignal(fileUrl)
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            Action {
                text: qsTr("Open")
                onTriggered: bookFileDialog.open()
            }
        }
    }

    ScrollView {
        anchors.fill: parent
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        TextArea {
            id: bookText
            objectName: "bookText"
            readOnly: true
            textFormat: Text.RichText
            wrapMode: TextEdit.WordWrap
            background: Rectangle {
                color: "transparent"
            }
        }
    }
}
