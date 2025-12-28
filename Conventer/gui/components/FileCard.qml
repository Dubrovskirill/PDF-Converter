import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: cardRoot
    property string fileName: "image.png"
    property string fileSize: "0.0 MB"
    property var previewSource: ""
    property bool isProcessing: false
    property bool isError: false

    signal removeClicked()

    // Фиксированный размер для сетки
    width: 160
    height: 200
    color: "white"
    radius: 10

    border.color: isError ? "#ff4d4d" : (mouseArea.containsMouse ? "#3498db" : "#e0e0e0")
    border.width: mouseArea.containsMouse ? 2 : 1

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 5

        // 1. БОЛЬШОЕ ПРЕВЬЮ
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#f8f9fa"
            radius: 6
            clip: true

            Image {
                anchors.fill: parent
                source: previewSource
                fillMode: Image.PreserveAspectCrop // Заполняем область
                visible: previewSource !== ""
                asynchronous: true
            }

            // Заглушка-иконка
            Text {
                anchors.centerIn: parent
                text: "📄"
                font.pixelSize: 40
                opacity: 0.2
                visible: previewSource === ""
            }

            // Кнопка удаления (появляется только при наведении)
            Button {
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: 4
                width: 24; height: 24
                visible: mouseArea.containsMouse

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
                onClicked: cardRoot.removeClicked()
            }

            BusyIndicator {
                anchors.centerIn: parent
                running: isProcessing
                visible: isProcessing
            }
        }

        // 2. ПОДПИСЬ
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

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
    }
}
