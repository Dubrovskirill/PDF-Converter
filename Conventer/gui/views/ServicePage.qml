import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import QtQml.Models 2.15
import "../components"

Item {
    id: root


    property string title: "Service"
    signal backRequested()
    property var imageFilters: ["Image files (*.jpg *.jpeg *.png)"]
    property var allFilesFilters: ["All supported (*.jpg *.jpeg *.png *.pdf)", "Image files (*.jpg *.jpeg *.png)", "PDF files (*.pdf)"]

    ListModel {
        id: filesModel
        ListElement { name: "Летний_отпуск_01.jpg"; size: "2.4 MB"; error: false; processing: false }
        ListElement { name: "Документ_со_сканера.pdf"; size: "15.1 MB"; error: false; processing: false }
        ListElement { name: "Ошибка_загрузки.png"; size: "0 KB"; error: true; processing: false }
        ListElement { name: "Обработка_фото.jpg"; size: "4.2 MB"; error: false; processing: true }
        ListElement { name: "Презентация.pdf"; size: "8.7 MB"; error: false; processing: false }
        ListElement { name: "Очень_длинное_название_файла_для_проверки_элайда.jpg"; size: "1.2 MB"; error: false; processing: false }
    }

    function logFileOrder() {
        console.log("--- Current File Order (Internal) ---");
        // Мы идем именно по visualModel, так как там хранится актуальный порядок
        for (var i = 0; i < visualModel.items.count; i++) {
            // Получаем объект из модели по индексу визуальной группы
            var item = visualModel.items.get(i).model;
            console.log(i + ": " + item.name);
        }
        console.log("-------------------------------------");
    }

    DelegateModel {
        id: visualModel
        model: filesModel
        delegate: FileCard {
            id: delegateItem
            fileName: model.name
            fileSize: model.size
            isError: model.error
            isProcessing: model.processing

            width: fileGrid.cellWidth
            height: fileGrid.cellHeight

            onRemoveClicked: {
                console.log("Removing item at index:", index)
                filesModel.remove(index)
            }

            DropArea {
                anchors.fill: parent
                keys: ["file_card"]

                onEntered: (drag) => {
                               // Проверяем, что у источника есть наш индекс
                               if (drag.source && drag.source.visualIndex !== undefined) {
                                   var from = drag.source.visualIndex;
                                   var to = delegateItem.DelegateModel.itemsIndex;

                                   if (from !== to) {
                                       visualModel.items.move(from, to);
                                   }
                               }
                           }
            }
        }
    }

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

        GridView {
            id: fileGrid
            Layout.fillWidth: true
            Layout.fillHeight: true

            cellWidth: 180
            cellHeight: 220
            clip: true



            DropZone {
                anchors.fill: parent
                z: -1
            }
            model: visualModel
            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded
            }

            move: Transition {
                NumberAnimation { properties: "x,y"; duration: 200; easing.type: Easing.OutQuad }
            }

            displaced: Transition {
                NumberAnimation { properties: "x,y"; duration: 200; easing.type: Easing.OutQuad }
            }
        }

        ServiceFooter {
            id: serviceFooter

            Layout.fillWidth: true
            actionText: root.title === "Картинки в PDF" ? "Convert to PDF" : "Merge PDF"

            isFinished: progress >= 1.0

            onActionClicked: {
                root.logFileOrder(); // Проверяем порядок перед "обработкой"

                console.log("Action started for: " + root.title);
                progress = 1.0;
                statusText = "Completed! Files saved to output folder.";
            }

            onOpenFolderClicked: {
                console.log("Opening folder with results...")
            }
        }
    }
}
