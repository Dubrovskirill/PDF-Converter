import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import QtQml.Models 2.15
import "../components"
import ".."


Item {
    id: root

    property string title: "Service"
    signal backRequested()
    property bool sortAscending: true

    property real progress: 0.0
    property string statusText: "Готов к работе"
    readonly property var imgExts: ["jpg", "jpeg", "png"]
    readonly property var pdfExts: ["pdf"]
    readonly property var allExts: imgExts.concat(pdfExts)

    property var imageFilters: ["Image files (*." + imgExts.join(" *.") + ")"]
    property var allFilesFilters: [
        "All supported (*." + allExts.join(" *.") + ")",
        "Image files (*." + imgExts.join(" *.") + ")",
        "PDF files (*." + pdfExts.join(" *.") + ")"
    ]
    readonly property var currentAllowedExts: title === "Картинки в PDF" ? imgExts : allExts


    ListModel {
        id: filesModel
        ListElement { name: "Летний_отпуск_01.jpg"; size: "2.4 MB"; error: false; processing: false }
        ListElement { name: "Документ_со_сканера.pdf"; size: "15.1 MB"; error: false; processing: false }
        ListElement { name: "Ошибка_загрузки.png"; size: "0 KB"; error: true; processing: false }
        ListElement { name: "Обработка_фото.jpg"; size: "4.2 MB"; error: false; processing: true }
        ListElement { name: "Презентация.pdf"; size: "8.7 MB"; error: false; processing: false }
        ListElement { name: "Очень_длинное_название_файла_для_проверки_элайда.jpg"; size: "1.2 MB"; error: false; processing: false }
    }


    function sortModel() {
        var data = []
        for (var i = 0; i < filesModel.count; i++) {
            var item = filesModel.get(i)

            data.push({
                          "name": item.name,
                          "size": item.size,
                          "error": item.error,
                          "processing": item.processing
                      })
        }

        data.sort(function(a, b) {

            var result = a.name.localeCompare(b.name);
            return root.sortAscending ? result : -result;
        })

        filesModel.clear()
        for (var j = 0; j < data.length; j++) {
            filesModel.append(data[j])
        }


        root.sortAscending = !root.sortAscending
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
        spacing: 10

        // --- ПЕРВАЯ СТРОКА ---
        RowLayout {
            Layout.fillWidth: true
            spacing: 15

            ToolButton {
                text: "← Назад"
                onClicked: root.backRequested()
            }

            Text {
                text: root.title
                font.pixelSize: 24; font.bold: true
                color: Style.textMain
                Layout.fillWidth: true
            }

            PrimaryButton {
                text: "Добавить файлы"
                iconText: "+"
                onClicked: fileDialog.open()
            }
        }

        // --- ВТОРАЯ СТРОКА ---
        RowLayout {
            Layout.fillWidth: true
            spacing: 20
            visible: filesModel.count > 0


            RowLayout {
                id: mergeControl
                spacing: 8
                visible: root.title === "Картинки в PDF"
                Layout.alignment: Qt.AlignVCenter


                property bool isHovered: mouseAreaMerge.containsMouse

                CheckBox {
                    id: mergeCheck
                    checked: false
                    padding: 0
                    hoverEnabled: false
                    Layout.alignment: Qt.AlignVCenter

                    indicator: Rectangle {
                        implicitWidth: 18
                        implicitHeight: 18
                        radius: 4

                        anchors.verticalCenter: parent.verticalCenter

                        color: mergeCheck.checked ? Style.primary : "transparent"
                        border.color: mergeCheck.checked ? Style.primary :
                                                           mergeControl.isHovered ? Style.primary : Style.borderDefault
                        border.width: 1

                        Behavior on border.color { ColorAnimation { duration: 150 } }
                        Behavior on color { ColorAnimation { duration: 150 } }

                        Text {
                            anchors.centerIn: parent
                            text: "✓"
                            color: "white"
                            visible: mergeCheck.checked
                            font.pixelSize: 12
                            font.bold: true
                        }
                    }
                }

                Text {
                    text: "Объединить в один PDF"
                    font.pixelSize: 13
                    Layout.alignment: Qt.AlignVCenter
                    color: mergeControl.isHovered || mergeCheck.checked ? Style.primary : Style.textSecondary

                    Behavior on color { ColorAnimation { duration: 150 } }
                }

                MouseArea {
                    id: mouseAreaMerge
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: mergeCheck.checked = !mergeCheck.checked
                }
            }

            Item { Layout.fillWidth: true }

            ToolButton {
                text: root.sortAscending ? "Имя A-Z ↓" : "Имя Z-A ↑"
                visible: filesModel.count > 1
                onClicked: sortModel()
            }

            ToolButton {
                text: "Очистить список"
                hoverColor: Style.danger
                onClicked: filesModel.clear()
            }
        }

        GridView {
            id: fileGrid
            Layout.fillWidth: true
            Layout.fillHeight: true
            cellWidth: 180
            cellHeight: 220
            clip: true


            property alias dropZoneId: dropZoneLogic

            DropZone {
                id: dropZoneLogic
                anchors.fill: parent
                z: -1
                allowedExtensions: root.currentAllowedExts
            }

            model: visualModel
            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

            move: Transition { NumberAnimation { properties: "x,y"; duration: 200; easing.type: Easing.OutQuad } }
            displaced: Transition { NumberAnimation { properties: "x,y"; duration: 200; easing.type: Easing.OutQuad } }

            Column {
                id: emptyState
                anchors.centerIn: parent
                spacing: 15

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
            actionText: root.title === "Картинки в PDF" ? "Конвертировать" : "Объединить"
            progress: root.progress
            statusText: root.statusText
            isFinished: root.progress >= 1.0

            onActionClicked: {
                root.statusText = "Обработка..."

                root.progress = 1.0
                root.statusText = "Готово!"
            }

            onOpenFolderClicked: {
                console.log("Открываем папку с результатом...")
            }
        }
    }

    Rectangle {
        id: dropOverlay

        x: fileGrid.x + 20
        y: fileGrid.y + 20
        width: fileGrid.width
        height: fileGrid.height

        z: 100
        radius: 10


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
