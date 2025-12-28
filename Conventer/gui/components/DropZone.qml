import QtQuick 2.15
import QtQuick.Controls 2.15

DropArea {
    id: dropArea
    anchors.fill: parent

    property bool containsDrag: false
    property int validCount: 0
    property int invalidCount: 0

    readonly property var allowedExtensions: root.title === "Картинки в PDF"
                                             ? ["jpg", "jpeg", "png"]
                                             : ["jpg", "jpeg", "png", "pdf"]

    Rectangle {
        id: dropOverlay
        anchors.fill: parent
        // Если есть хоть один плохой файл — оранжевый (предупреждение), если только хорошие — синий
        color: invalidCount > 0 ? "#f39c12" : "#3498db"
        opacity: dropArea.containsDrag ? 0.2 : 0
        border.color: invalidCount > 0 ? "#f39c12" : "#3498db"
        border.width: dropArea.containsDrag ? 4 : 0
        radius: 10
        visible: opacity > 0

        Behavior on opacity { NumberAnimation { duration: 150 } }

        Column {
            anchors.centerIn: parent
            spacing: 15
            Text {
                text: invalidCount > 0 ? "⚠️" : "📥"
                font.pixelSize: 60
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Text {
                text: invalidCount > 0
                      ? "Будет добавлено " + validCount + " файл(ов). " + invalidCount + " пропущено."
                      : "Отпустите для добавления " + validCount + " файл(ов)"
                font.pixelSize: 18
                font.bold: true
                color: invalidCount > 0 ? "#e67e22" : "#2980b9"
            }
        }
    }

    function processUrls(urls, executeAdd) {
        var accepted = [];
        var rejected = [];

        for (var i = 0; i < urls.length; i++) {
            var path = urls[i].toString();
            var ext = path.split('.').pop().toLowerCase();

            if (allowedExtensions.indexOf(ext) !== -1) {
                accepted.push(path);
            } else {
                rejected.push(path.split('/').pop()); // Сохраняем только имя файла
            }
        }

        if (executeAdd) {
            if (accepted.length > 0) {
                console.log("Adding files:", accepted);
                // Future call: viewModel.addFiles(accepted)
            }
            if (rejected.length > 0) {
                console.warn("Files rejected due to format:", rejected.join(", "));
            }
        }

        validCount = accepted.length;
        invalidCount = rejected.length;
        return accepted.length > 0;
    }

    onEntered: (drag) => {
                   containsDrag = true
                   processUrls(drag.urls, false)
               }

    onPositionChanged: (drag) => {
                           processUrls(drag.urls, false)
                       }

    onExited: {
        containsDrag = false
        validCount = 0
        invalidCount = 0
    }

    onDropped: (drop) => {
                   containsDrag = false
                   if (drop.hasUrls) {
                       var canAccept = processUrls(drop.urls, true);
                       if (canAccept) drop.acceptProposedAction();
                   }
                   validCount = 0
                   invalidCount = 0
               }
}
