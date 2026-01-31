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

    property real progress: converterVM.progress
    property string statusText: converterVM.statusText

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


    function sortModel() {
        // Вызываем C++ метод
        converterVM.sortByName(root.sortAscending)

        // Переключаем флаг для следующего нажатия
        root.sortAscending = !root.sortAscending
    }

    DelegateModel {
        id: visualModel
        model: fileModel
        delegate: FileCard {
            id: delegateItem
            // ИСПОЛЬЗУЕМ РОЛИ ИЗ C++ (PdfFileModel.cpp):
            fileName: model.fileName
            fileSize: model.fileSize
            isError: model.isError
            isProcessing: model.isProcessing
            previewSource: model.previewSource

            width: fileGrid.cellWidth
            height: fileGrid.cellHeight

            onRemoveClicked: {
                // ВЫЗОВ C++:
                converterVM.removeFile(index)
            }

            DropArea {
                anchors.fill: parent
                keys: ["file_card"]

                onEntered: function(drag) {
                    if (drag.source && drag.source.visualIndex !== undefined) {
                        var from = drag.source.visualIndex;
                        var to = delegateItem.DelegateModel.itemsIndex;
                        if (from !== to) {

                            converterVM.moveFile(from, to)
                        }
                    }
                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Выберите файлы"
        folder: shortcuts.pictures
        selectMultiple: true
        nameFilters: root.title === "Картинки в PDF" ? imageFilters : allFilesFilters

        onAccepted: {

            converterVM.addFiles(fileUrls)
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
                onClicked: {
                        converterVM.clearList()
                        root.backRequested()
                    }
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
            visible: visualModel.items.count > 0


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
                    onCheckedChanged: {
                            converterVM.resetProcessingStatus()
                        }
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
                visible: visualModel.items.count > 1
                onClicked: sortModel()
            }

            ToolButton {
                text: "Очистить список"
                hoverColor: Style.danger
                onClicked: converterVM.clearList()
            }
        }

        GridView {
            id: fileGrid
            Layout.fillWidth: true
            Layout.fillHeight: true
            cellWidth: Style.cardWidth + 10
            cellHeight: (Style.cardWidth * Style.ratioA4) + 70
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
            progress: converterVM.progress
            statusText: converterVM.statusText
            isFinished: converterVM.isFinished

            onActionClicked: {
                var serviceType = (root.title === "Картинки в PDF") ? "imagesToPdf" : "allToPdf"

                converterVM.runConversion(serviceType, mergeCheck.checked)
            }

            onOpenFolderClicked: {
                converterVM.openResultFolder()
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
