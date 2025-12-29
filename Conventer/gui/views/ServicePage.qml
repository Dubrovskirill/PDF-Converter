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
        for (var i = 0; i < visualModel.items.count; i++) {
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

                onEntered: function(drag) {
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
                CheckBox { id: mergeCheck; checked: false; text: ""; implicitWidth: 30 }
                Text {
                    text: "Объединить в один PDF"
                    font.pixelSize: 14
                    color: "#333"
                    MouseArea { anchors.fill: parent; onClicked: mergeCheck.checked = !mergeCheck.checked }
                }
            }

            Item { Layout.fillWidth: true }
            Button {
                text: "Очистить всё"

                visible: filesModel.count > 0

                contentItem: Text {
                    text: parent.text
                    color: "red"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    console.log("QML: Clearing all files from model")
                    filesModel.clear()
                }
            }
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

            // Пробрасываем ID наружу для оверлея
            property alias dropZoneId: dropZoneLogic

            DropZone {
                id: dropZoneLogic
                anchors.fill: parent
                z: -1
            }

            model: visualModel
            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

            move: Transition { NumberAnimation { properties: "x,y"; duration: 200; easing.type: Easing.OutQuad } }
            displaced: Transition { NumberAnimation { properties: "x,y"; duration: 200; easing.type: Easing.OutQuad } }

            Column {
                id: emptyState
                anchors.centerIn: parent
                spacing: 15
                // Отображаем, только если в модели 0 элементов и сейчас не идет перетаскивание
                visible: visualModel.items.count === 0 && !dropZoneLogic.containsDrag
                opacity: 0.5

                Text {
                    text: "📥"
                    font.pixelSize: 80
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Text {
                    text: "Перетащите файлы сюда"
                    font.pixelSize: 20
                    font.bold: true
                    color: "#2c3e50"
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Text {
                    text: "или используйте кнопку «Добавить файлы»"
                    font.pixelSize: 14
                    color: "#7f8c8d"
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }



        }

        ServiceFooter {
            id: serviceFooter
            Layout.fillWidth: true
            actionText: root.title === "Картинки в PDF" ? "Convert to PDF" : "Merge PDF"
            isFinished: progress >= 1.0
            onActionClicked: {
                root.logFileOrder();
                progress = 1.0;
                statusText = "Completed!";
            }
        }
    }

    Rectangle {
        id: dropOverlay
        // Позиционирование относительно fileGrid
        x: fileGrid.x + 20
        y: fileGrid.y + 20
        width: fileGrid.width
        height: fileGrid.height

        z: 100
        radius: 10

        // Прямое обращение к ID DropZone для исключения ReferenceError
        visible: dropZoneLogic.containsDrag && dropZoneLogic.isFileDrag
        color: dropZoneLogic.invalidCount > 0 ? "#f39c12" : "#3498db"
        opacity: 0.2

        border.color: dropZoneLogic.invalidCount > 0 ? "#f39c12" : "#3498db"
        border.width: 4

        Column {
            anchors.centerIn: parent
            spacing: 15
            Text {
                text: dropZoneLogic.invalidCount > 0 ? "⚠️" : "📥"
                font.pixelSize: 60
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Text {
                text: dropZoneLogic.invalidCount > 0
                      ? "Будет добавлено " + dropZoneLogic.validCount + " файл(ов). " + dropZoneLogic.invalidCount + " пропущено."
                      : "Отпустите для добавления " + dropZoneLogic.validCount + " файл(ов)"
                font.pixelSize: 18
                font.bold: true
                color: dropZoneLogic.invalidCount > 0 ? "#e67e22" : "#2980b9"
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
