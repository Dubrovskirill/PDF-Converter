import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import "../components"

Item {
    id: root
    property string title: "Service"
    signal backRequested()
    property var imageFilters: ["Image files (*.jpg *.jpeg *.png)"]
    property var allFilesFilters: ["All supported (*.jpg *.jpeg *.png *.pdf)", "Image files (*.jpg *.jpeg *.png)", "PDF files (*.pdf)"]

    FileDialog {
        id: fileDialog
        title: "Please choose files"
        folder: shortcuts.pictures
        selectMultiple: true
        nameFilters: root.title === "Картинки в PDF" ? imageFilters : allFilesFilters

        onAccepted: {
            console.log("Selected files from dialog:")
            for (var i = 0; i < fileUrls.length; i++) {
                console.log("- " + fileUrls[i])
            }
            // Здесь позже: viewModel.addFiles(fileUrls)
        }
        onRejected: {
            console.log("Canceled")
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        RowLayout {
            Layout.fillWidth: true
            Button {
                text: "<- Назад"
                onClicked: root.backRequested()
            }

            Text {
                text: root.title
                font.pixelSize: 24
                font.bold: true
                Layout.leftMargin: 10
            }
            RowLayout {
                visible: root.title === "Картинки в PDF"
                spacing: 10
                Layout.leftMargin: 20

                CheckBox {
                    id: mergeCheck
                    checked: false
                    text: ""
                    implicitWidth: 30
                }

                Text {
                    text: "Объединить в один PDF"
                    font.pixelSize: 14
                    color: "#333"
                    verticalAlignment: Text.AlignVCenter

                    MouseArea {
                        anchors.fill: parent
                        onClicked: mergeCheck.checked = !mergeCheck.checked
                    }
                }
            }
            Item { Layout.fillWidth: true }

            Button {
                text: "+ Добавить файлы"
                highlighted: true
                onClicked: fileDialog.open()
            }
        }

        // Рабочая область с сеткой
        GridView {
            id: fileGrid
            Layout.fillWidth: true
            Layout.fillHeight: true

            cellWidth: 180
            cellHeight: 220
            clip: true

            model: ListModel {
                ListElement { name: "Летний_отпуск_01.jpg"; size: "2.4 MB"; error: false; processing: false }
                ListElement { name: "Документ_со_сканера.pdf"; size: "15.1 MB"; error: false; processing: false }
                ListElement { name: "Ошибка_загрузки.png"; size: "0 KB"; error: true; processing: false }
                ListElement { name: "Обработка_фото.jpg"; size: "4.2 MB"; error: false; processing: true }
                ListElement { name: "Презентация.pdf"; size: "8.7 MB"; error: false; processing: false }
                ListElement { name: "Очень_длинное_название_файла_для_проверки_элайда.jpg"; size: "1.2 MB"; error: false; processing: false }
            }

            delegate: FileCard {
                fileName: model.name
                fileSize: model.size
                isError: model.error
                isProcessing: model.processing
                onRemoveClicked: {
                    console.log("Removing item at index:", index)
                    // Здесь позже будет логика удаления из реальной модели
                }
            }

            DropZone {
                anchors.fill: parent
            }

            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded
            }
        }

        ServiceFooter {
            Layout.fillWidth: true
            actionText: root.title === "Картинки в PDF" ? "Convert to PDF" : "Merge PDF"

            onActionClicked: {
                console.log("Action started for: " + root.title)
            }
        }
    }
}
