import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root
    property string title: "Service"
    signal backRequested()

    Column {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        Button {
            text: "<- Назад"
            onClicked: root.backRequested()
        }

        Text {
            text: root.title
            font.pixelSize: 24
            font.bold: true
        }

        // Тут позже появится наш ListView и DropArea
        Rectangle {
            width: parent.width
            height: 300
            color: "#f0f0f0"
            border.color: "#ccc"
            Text { text: "Рабочая область"; anchors.centerIn: parent }
        }
    }
}
