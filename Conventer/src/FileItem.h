#ifndef FILEITEM_H
#define FILEITEM_H

#include <QString>
#include <QImage>
#include <QFileInfo>


struct FileItem {
    QString filePath;
    QString fileName;
    QString fileSize;
    QImage preview;
    bool isProcessing;
    bool isError;
    int progress;


    QString suffix() const {
        return QFileInfo(filePath).suffix().toLower();
    }
};

#endif // FILEITEM_H
