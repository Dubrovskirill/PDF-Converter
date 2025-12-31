import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import ".." // Чтобы видеть Style

Button {
    id: control
    property color hoverColor: Style.primary
    property color textColor: Style.textSecondary

    Layout.preferredHeight: 32

    contentItem: Text {
        text: control.text
        font.pixelSize: 13
        color: control.hovered ? control.hoverColor : control.textColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        Behavior on color { ColorAnimation { duration: 150 } }
    }

    background: Rectangle {
        color: control.pressed ? Qt.alpha(control.hoverColor, 0.05) : "transparent"
        border.color: control.hovered ? control.hoverColor : Style.borderDefault
        border.width: 1
        radius: Style.radius
        Behavior on border.color { ColorAnimation { duration: 150 } }
    }
}
