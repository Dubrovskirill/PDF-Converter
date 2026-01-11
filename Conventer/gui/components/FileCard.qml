import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQml.Models 2.15
import ".." // Явный импорт, чтобы увидеть Style.qml

Item {
    id: root
    property string fileName: "image.png"
    property string fileSize: "0.0 MB"
    property var previewSource: ""
    property bool isProcessing: false
    property bool isError: false

    signal removeClicked()

    readonly property int visualIndex: DelegateModel.itemsIndex

    width: 160
    height: 200

    Rectangle {
        id: cardRoot
        width: 160
        height: 200
        color: Style.bgMain
        radius: Style.radius


        border.width: 1
        border.color: (mouseArea.containsMouse || mouseArea.drag.active || removeButton.hovered)
                      ? Style.borderHover
                      : Style.borderDefault


        Drag.active: mouseArea.drag.active
        Drag.source: root
        Drag.keys: ["file_card"]
        Drag.hotSpot.x: width / 2
        Drag.hotSpot.y: height / 2

        states: [
            State {
                when: mouseArea.drag.active
                ParentChange { target: cardRoot; parent: fileGrid }
                PropertyChanges {
                    target: cardRoot;
                    opacity: 0.8;
                    scale: 1.1;
                    z: 1000
                }
            }
        ]

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 8
            spacing: 5

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: Style.bgLight
                radius: 6
                clip: true

                Image {
                    anchors.fill: parent
                    source: previewSource
                    fillMode: Image.PreserveAspectCrop
                    visible: status === Image.Ready && !isProcessing && !isError
                    asynchronous: true
                    cache: false
                }

                Text {
                    anchors.centerIn: parent
                    text: Style.iconFile
                    font.pixelSize: 40
                    opacity: 0.2
                    visible: (!isProcessing && previewSource === "") || isError
                }

                BusyIndicator {
                    anchors.centerIn: parent
                    running: isProcessing
                    visible: isProcessing
                }
            }

            Text {
                Layout.fillWidth: true
                text: fileName
                font.pixelSize: 12
                font.bold: true
                color: Style.textMain
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                Layout.fillWidth: true
                text: isError ? "Ошибка" : fileSize
                font.pixelSize: 10
                color: isError ? Style.danger : Style.textSecondary
                horizontalAlignment: Text.AlignHCenter
            }
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            drag.target: cardRoot

            onReleased: {
                if (cardRoot.Drag.active) {
                    cardRoot.Drag.drop()
                }
            }
        }


        Button {
            id: removeButton
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.margins: 12
            width: 24; height: 24
            z: 2

            visible: (mouseArea.containsMouse || hovered) && !mouseArea.drag.active

            background: Rectangle {
                radius: 12
                color: removeButton.hovered ? Style.bgLight : Style.bgMain
                border.color: Style.borderDefault
            }
            contentItem: Text {
                text: Style.iconDelete
                color: Style.danger
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            onClicked: {
                console.log("QML: Clicked remove button on card:", fileName)
                root.removeClicked()
            }
        }
    }
}
