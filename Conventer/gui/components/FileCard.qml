import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQml.Models 2.15

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
        color: "white"
        radius: 10

        Drag.active: mouseArea.drag.active
        Drag.source: root
        Drag.keys: ["file_card"]
        Drag.hotSpot.x: width / 2
        Drag.hotSpot.y: height / 2

        states: [
            State {
                when: mouseArea.drag.active
                ParentChange { target: cardRoot; parent: fileGrid } // Чтобы летала над всем GridView
                PropertyChanges { target: cardRoot; opacity: 0.8; scale: 1.1; z: 1000 }
            }
        ]

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 8
            spacing: 5

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#f8f9fa"
                radius: 6
                clip: true

                Image {
                    anchors.fill: parent
                    source: previewSource
                    fillMode: Image.PreserveAspectCrop
                    visible: previewSource !== ""
                    asynchronous: true
                }

                Text {
                    anchors.centerIn: parent
                    text: "📄"
                    font.pixelSize: 40
                    opacity: 0.2
                    visible: previewSource === ""
                }

                Button {
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.margins: 4
                    width: 24; height: 24

                    visible: mouseArea.containsMouse && !mouseArea.drag.active

                    background: Rectangle {
                        radius: 12
                        color: "white"
                        border.color: "#ddd"
                    }
                    contentItem: Text {
                        text: "✕"; color: "red";
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: root.removeClicked()
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
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                Layout.fillWidth: true
                text: isError ? "Ошибка" : fileSize
                font.pixelSize: 10
                color: isError ? "red" : "#888"
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        drag.target: cardRoot

        // Добавь это:
        onReleased: {
            if (cardRoot.Drag.active) {
                cardRoot.Drag.drop() // Фиксируем сброс
            }
        }
    }
}
