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
    ~QtImageConverter() override = default;

    bool batchConvertToPdf(const QStringList &imageFiles, const QString &outputDir) override
    {
        if (imageFiles.isEmpty()) return false;

        QDir dir;
        if (!dir.exists(outputDir) && !dir.mkpath(outputDir)) {
            return false;
        }

        bool overallSuccess = true;

        for (const QString &imagePath : imageFiles) {
            QImage img(imagePath);
            if (img.isNull()) {
                overallSuccess = false;
                continue;
            }

            QString outFileName = outputDir + "/" + QFileInfo(imagePath).completeBaseName() + ".pdf";

            QPdfWriter writer(outFileName);
            writer.setPageLayout(QPageLayout(QPageSize(QPageSize::A4),
                                             QPageLayout::Portrait,
                                             QMarginsF(0, 0, 0, 0)));
            writer.setResolution(300);

            QPainter painter(&writer);
            if (!painter.isActive()) {
                overallSuccess = false;
                continue;
            }

            QRect targetRect = writer.pageLayout().paintRectPixels(writer.resolution());
            QImage scaledImg = img.scaled(targetRect.size(),
                                          Qt::KeepAspectRatio,
                                          Qt::SmoothTransformation);

            int x = (targetRect.width() - scaledImg.width()) / 2;
            int y = (targetRect.height() - scaledImg.height()) / 2;

            painter.drawImage(x, y, scaledImg);
            painter.end();
        }

        return overallSuccess;
    }
};
#endif // QTIMAGECONVERTER_H
