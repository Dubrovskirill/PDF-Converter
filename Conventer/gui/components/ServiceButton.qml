import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import ".."

Button {
    id: control

    property string iconSymbol: ""


    implicitWidth: 320
    implicitHeight: 100

    contentItem: RowLayout {

        spacing: 20
        Layout.alignment: Qt.AlignVCenter

        Text {
            text: control.iconSymbol
            font.pixelSize: 42
            leftPadding: 20
            Layout.alignment: Qt.AlignVCenter
        }

        Text {
            text: control.text
            font.pixelSize: 20
            font.bold: true
            color: Style.textMain
            Layout.fillWidth: true
            verticalAlignment: Text.AlignVCenter
        }
    }

    background: Rectangle {
        color: control.pressed ? Qt.darker(Style.bgLight, 1.05) :
               control.hovered ? Qt.darker(Style.bgLight, 1.02) : Style.bgMain
        radius: Style.radius * 2

        border.color: control.hovered ? Style.primary : Style.borderDefault
        border.width: control.hovered ? 2 : 1


        layer.enabled: control.hovered

        Behavior on border.color { ColorAnimation { duration: 150 } }
        Behavior on color { ColorAnimation { duration: 150 } }
    }
}
