import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: footerRoot
    height: 70
    color: "#f8f9fa"
    border.color: "#e0e0e0"
    border.width: 1

    property double progress: 0.0
    property string statusText: "Ready to work"
    property string actionText: "Convert"
    property bool isFinished: false

    signal actionClicked()
    signal openFolderClicked()

    RowLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 20

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 5

            Text {
                id: statusLabel
                text: footerRoot.statusText
                font.pixelSize: 12
                color: "#666"
            }

            ProgressBar {
                id: progressBar
                Layout.fillWidth: true
                value: footerRoot.progress

                background: Rectangle {
                    implicitHeight: 8
                    color: "#e9ecef"
                    radius: 4
                }
                contentItem: Item {
                    Rectangle {
                        width: progressBar.visualPosition * parent.width
                        height: parent.height
                        radius: 4
                        color: footerRoot.isFinished ? "#2ecc71" : "#3498db"
                    }
                }
            }
        }


        Button {
            id: folderButton
            text: "📁"
            visible: footerRoot.isFinished
            Layout.preferredWidth: 40
            Layout.preferredHeight: 40

            background: Rectangle {
                color: folderButton.pressed ? "#d5d8dc" : (folderButton.hovered ? "#ebedef" : "#f4f6f7")
                radius: 6
                border.color: "#bdc3c7"
            }

            onClicked: footerRoot.openFolderClicked()
        }

        Button {
            id: actionButton
            text: footerRoot.actionText
            Layout.preferredWidth: 150
            Layout.preferredHeight: 40

            contentItem: Text {
                text: actionButton.text
                font.bold: true
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: actionButton.pressed ? "#2471a3" : (actionButton.hovered ? "#2e86c1" : "#3498db")
                radius: 6
            }

            onClicked: footerRoot.actionClicked()
        }
    }
}
