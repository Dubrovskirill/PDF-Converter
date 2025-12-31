import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import ".."

Button {
    id: control
    property string iconText: ""

    Layout.preferredHeight: 40
    Layout.preferredWidth: 180

    contentItem: Item {
        Row {
            anchors.centerIn: parent
            spacing: 10

            Rectangle {
                width: 18; height: 18; radius: 4; color: "white"
                visible: control.iconText !== ""
                anchors.verticalCenter: parent.verticalCenter
                Text {
                    text: control.iconText
                    anchors.centerIn: parent
                    color: Style.primary
                    font.bold: true
                    font.pixelSize: 14
                }
            }
            Text {
                text: control.text
                font.bold: true; font.pixelSize: 13; color: "white"
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    background: Rectangle {
        color: control.pressed ? Style.primaryDark :
               control.hovered ? Qt.lighter(Style.primary, 1.1) : Style.primary
        radius: Style.radius
        Behavior on color { ColorAnimation { duration: 150 } }
    }
}
