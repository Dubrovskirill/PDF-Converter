import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../components"
import ".."

Item {
    id: root
    signal serviceSelected(string serviceType)

    Rectangle {
        anchors.fill: parent
        color: Style.bgLight
        z: -1
    }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 40

        // Заголовок
        ColumnLayout {
            spacing: 12
            Layout.alignment: Qt.AlignHCenter

            Text {
                text: "PDF Converter"
                font.pixelSize: 42
                font.bold: true
                color: Style.textMain
                Layout.alignment: Qt.AlignHCenter
            }

            Rectangle {
                Layout.preferredWidth: 80
                Layout.preferredHeight: 4
                color: Style.primary
                radius: 2
                Layout.alignment: Qt.AlignHCenter
            }
        }

        // Список сервисов
        ColumnLayout {
            spacing: 15
            Layout.alignment: Qt.AlignHCenter

            ServiceButton {
                text: "Картинки в PDF"
                iconSymbol: Style.iconImagesToPdf
                onClicked: root.serviceSelected("imagesToPdf")
            }

            ServiceButton {
                text: "Объединить PDF"
                iconSymbol: Style.iconMergePdf
                onClicked: root.serviceSelected("allToPdf")
            }

            ServiceButton {
                text: "Редактирование"
                iconSymbol: Style.iconEdit
                enabled: false
                opacity: 0.5
            }
        }
    }
}
