import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import "views" // Подключаем папку с вьюшками

Window {
    width: 800
    height: 600
    minimumWidth: 800
    minimumHeight: 600
    visible: true
    title: qsTr("PDF Converter")

    StackView {
        id: mainStack
        anchors.fill: parent
        initialItem: mainMenu
    }

    Component {
        id: mainMenu
        MainMenu {
            onServiceSelected: {
                if (serviceType === "imagesToPdf") {
                    mainStack.push(servicePage, { title: "Картинки в PDF" })
                } else if (serviceType === "allToPdf") {
                    mainStack.push(servicePage, { title: "Объеденить PDF" })
                }
            }
        }
    }

    Component {
        id: servicePage
        ServicePage {
            onBackRequested: mainStack.pop()
        }
    }
}
