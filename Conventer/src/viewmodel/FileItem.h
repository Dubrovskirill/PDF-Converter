#ifndef FILEITEM_H
#define FILEITEM_H

#include <QString>
#include <QImage>

struct FileItem {
    QString filePath;
    QString fileName;
    QString fileSize;
    QImage preview;
    bool isProcessing = false;
    bool isError = false;
    QString statusText;
};

#endif // FILEITEM_H
