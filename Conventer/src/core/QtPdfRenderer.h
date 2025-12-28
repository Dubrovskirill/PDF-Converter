#ifndef QTPDFRENDERER_H
#define QTPDFRENDERER_H

#include "interfaces/IPdfRenderer.h"
#include <poppler-qt5.h>
#include <QFileInfo>
#include <QImageReader>
#include <QDebug>
#include <memory>

class QtPdfRenderer : public IPdfRenderer {
public:
    QImage renderPreview(const QString &filePath, int pageIndex = 0, QSize targetSize = QSize(200, 200)) override {
        QFileInfo fileInfo(filePath);
        QString ext = fileInfo.suffix().toLower();

        if (ext == "pdf") {
            return renderPdfPage(filePath, pageIndex, targetSize);
        } else {
            return renderImage(filePath, targetSize);
        }
    }

    int getPageCount(const QString &filePath) override {
        QFileInfo fileInfo(filePath);
        if (fileInfo.suffix().toLower() != "pdf") return 1;
        std::unique_ptr<Poppler::Document> doc(Poppler::Document::load(filePath));
        if (!doc || doc->isLocked()) {
            return 0;
        }
        return doc->numPages();
    }

private:
    QImage renderPdfPage(const QString &filePath, int pageIndex, QSize targetSize) {
        std::unique_ptr<Poppler::Document> doc(Poppler::Document::load(filePath));
        if (!doc || doc->isLocked()) return QImage();

        if (pageIndex < 0 || pageIndex >= doc->numPages()) return QImage();

        std::unique_ptr<Poppler::Page> page(doc->page(pageIndex));
        if (!page) return QImage();

        double dpiX = 72.0;
        double dpiY = 72.0;

        if (targetSize.isValid()) {
            QSizeF pageSize = page->pageSizeF();
            if (pageSize.width() > 0 && pageSize.height() > 0) {
                dpiX = (targetSize.width() / pageSize.width()) * 72.0;
                dpiY = (targetSize.height() / pageSize.height()) * 72.0;

                double finalDpi = qMin(dpiX, dpiY);
                return page->renderToImage(finalDpi, finalDpi);
            }
        }

        return page->renderToImage(dpiX, dpiY);
    }

    QImage renderImage(const QString &filePath, QSize targetSize) {
        QImageReader reader(filePath);
        if (!reader.canRead()) return QImage();

        if (targetSize.isValid()) {
            QSize imageSize = reader.size();
            imageSize.scale(targetSize, Qt::KeepAspectRatio);
            reader.setScaledSize(imageSize);
        }

        return reader.read();
    }
};

#endif // QTPDFRENDERER_H
