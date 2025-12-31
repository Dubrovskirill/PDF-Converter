import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import ".."

Rectangle {
    id: footerRoot
    height: 85
    color: Style.bgLight
    border.color: Style.borderDefault
    border.width: 1
    radius: Style.radius

    property double progress: 0.0
    property string statusText: ""
    property string actionText: ""
    property bool isFinished: false

    signal actionClicked()
    signal openFolderClicked()

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 25


        ColumnLayout {
            Layout.fillWidth: true
            spacing: 8

            Text {
                id: statusLabel
                text: footerRoot.statusText
                font.pixelSize: 13
                font.weight: Font.Medium
                color: Style.textMain
            }

            ProgressBar {
                id: progressBar
                Layout.fillWidth: true
                value: footerRoot.progress

                Behavior on value {
                    NumberAnimation { duration: 400; easing.type: Easing.OutCubic }
                }

                background: Rectangle {
                    implicitHeight: 10
                    color: Style.borderDefault
                    radius: 5
                }

                contentItem: Item {
                    Rectangle {
                        width: progressBar.visualPosition * parent.width
                        height: parent.height
                        radius: 5
                        color: footerRoot.isFinished ? "#2ecc71" : Style.primary
                        Behavior on color { ColorAnimation { duration: 300 } }
                    }
                }
            }
        }



        Button {
            id: mainActionButton
            Layout.preferredWidth: 220
            Layout.preferredHeight: 45

            text: footerRoot.isFinished ? "Открыть результат" : footerRoot.actionText

            contentItem: Item {

                RowLayout {
                    id: contentRow
                    anchors.centerIn: parent
                    spacing: 10

                    Layout.fillWidth: false

                    Text {
                        text: Style.iconFolder
                        font.pixelSize: 20
                        color: "white"
                        visible: footerRoot.isFinished


                        verticalAlignment: Text.AlignVCenter
                        renderType: Text.NativeRendering
                    }

                    Text {
                        text: mainActionButton.text
                        font.pixelSize: 14
                        font.bold: true
                        color: "white"

                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        renderType: Text.NativeRendering
                    }
                }
            }

            background: Rectangle {
                color: mainActionButton.pressed ? Style.primaryDark :
                       (mainActionButton.hovered ?Qt.lighter(Style.primary, 1.1) : Style.primary)
                radius: Style.radius

                Behavior on color { ColorAnimation { duration: 150 } }
            }

            onClicked: {
                if (footerRoot.isFinished) {
                    footerRoot.openFolderClicked()
                } else {
                    footerRoot.actionClicked()
                }
            }
        }
    }
}
