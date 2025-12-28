import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: root
    signal serviceSelected(string serviceType)

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 20

        Text {
            text: "PDF Converter"
            font.pixelSize: 32
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
            bottomPadding: 20
        }

        Button {
            text: "Картинки в PDF"
            Layout.preferredWidth: 250
            Layout.preferredHeight: 60
            onClicked: root.serviceSelected("imagesToPdf")
        }

        Button {
            text: "Объеденить PDF"
            Layout.preferredWidth: 250
            Layout.preferredHeight: 60
            onClicked: root.serviceSelected("allToPdf")
        }

        Button {
            text: "Редактирование (Заглушка)"
            enabled: false
            Layout.preferredWidth: 250
            Layout.preferredHeight: 60
        }
    }
}
