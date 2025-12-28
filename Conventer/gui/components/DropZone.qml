import QtQuick 2.15
import QtQuick.Controls 2.15

DropArea {
    id: dropArea
    anchors.fill: parent

    property bool containsDrag: false

    Rectangle {
        id: dropOverlay
        anchors.fill: parent
        color: "#3498db"
        opacity: dropArea.containsDrag ? 0.2 : 0
        border.color: "#3498db"
        border.width: dropArea.containsDrag ? 4 : 0
        radius: 10
        visible: opacity > 0

        Behavior on opacity { NumberAnimation { duration: 150 } }

        Column {
            anchors.centerIn: parent
            spacing: 15

            Text {
                text: "📥"
                font.pixelSize: 60
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                text: "Отпустите файлы для добавления"
                font.pixelSize: 20
                font.bold: true
                color: "#2980b9"
            }
        }
    }

    onEntered: (drag) => {
        // Здесь можно проверить форматы: drag.hasUrls
        containsDrag = true
    }

    onExited: {
        containsDrag = false
    }

    onDropped: (drop) => {
        containsDrag = false
        if (drop.hasUrls) {
            console.log("Files received:")
            for (var i = 0; i < drop.urls.length; i++) {
                console.log("- " + drop.urls[i])
            }
            // Позже здесь будет вызов ViewModel: model.addFiles(drop.urls)
            drop.acceptProposedAction()
        }
    }
}
