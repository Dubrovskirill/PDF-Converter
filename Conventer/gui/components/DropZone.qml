import QtQuick 2.15
import QtQuick.Controls 2.15

DropArea {
    id: dropArea
    anchors.fill: parent

    // Эти свойства мы будем считывать снаружи для показа подсветки
    property bool containsDrag: false
    property bool isFileDrag: false
    property int validCount: 0
    property int invalidCount: 0

    property var allowedExtensions: []

    function processUrls(urls, executeAdd) {
        var accepted = [];
        var rejected = [];
        for (var i = 0; i < urls.length; i++) {
            var path = urls[i].toString();
            var ext = path.split('.').pop().toLowerCase();
            if (allowedExtensions.indexOf(ext) !== -1) {
                accepted.push(path);
            } else {
                rejected.push(path.split('/').pop());
            }
        }

        if (executeAdd && accepted.length > 0) {
            console.log("Adding files:", accepted);
            // Здесь будет вызов C++: viewModel.addFiles(accepted)
        }

        validCount = accepted.length;
        invalidCount = rejected.length;
        return accepted.length > 0;
    }

    onEntered: function(drag) {
        if (drag.hasUrls) {
            isFileDrag = true;
            containsDrag = true;
            processUrls(drag.urls, false);
        } else {
            isFileDrag = false;
        }
    }

    onPositionChanged: function(drag) {
        if (drag.hasUrls) {
            processUrls(drag.urls, false);
        }
    }

    onExited: {
        containsDrag = false;
        isFileDrag = false;
        validCount = 0;
        invalidCount = 0;
    }

    onDropped: function(drop) {
        containsDrag = false;
        isFileDrag = false;
        if (drop.hasUrls) {
            var canAccept = processUrls(drop.urls, true);
            if (canAccept) drop.acceptProposedAction();
        }
    }
}
