#ifndef QTIMAGECONVERTER_H
#define QTIMAGECONVERTER_H

#include <QPdfWriter>
#include <QPainter>
#include <QImage>
#include <QFileInfo>
#include <QDir>
#include <QPageLayout>
#include "interfaces/IImageConverter.h"

class QtImageConverter : public IImageConverter {
public:
    explicit QtImageConverter() = default;

    QString convertImageToPdf(const QString &imagePath, const QString &outputDir) override {
        QDir().mkpath(outputDir);
        QString outFileName = outputDir + "/" + QFileInfo(imagePath).completeBaseName() + ".pdf";

        if (saveToPdf(imagePath, outFileName)) {
            return outFileName;
        }
        return QString();
    }

    bool batchConvertToPdf(const QStringList &imageFiles, const QString &outputDir) override {
        if (imageFiles.isEmpty()) return false;
        bool allOk = true;

        for (const QString &path : imageFiles) {
            if (convertImageToPdf(path, outputDir).isEmpty()) {
                allOk = false;
            }
        }
        return allOk;
    }

private:
    // Внутренний метод, инкапсулирующий логику отрисовки
    bool saveToPdf(const QString &src, const QString &dest) {
        QImage img(src);
        if (img.isNull()) return false;

        QPdfWriter writer(dest);
        writer.setPageLayout(QPageLayout(QPageSize(QPageSize::A4),
                                         QPageLayout::Portrait,
                                         QMarginsF(0, 0, 0, 0)));
        writer.setResolution(900);

        QPainter painter(&writer);
        if (!painter.isActive()) return false;

        QRect targetRect = writer.pageLayout().paintRectPixels(writer.resolution());
        QImage scaledImg = img.scaled(targetRect.size(),
                                      Qt::KeepAspectRatio,
                                      Qt::SmoothTransformation);

        int x = (targetRect.width() - scaledImg.width()) / 2;
        int y = (targetRect.height() - scaledImg.height()) / 2;

        painter.drawImage(x, y, scaledImg);
        return painter.end();
    }
};

#endif // QTIMAGECONVERTER_H
