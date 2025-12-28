#include "PopplerPdfProcessor.h"
#include "qdebug.h"
#include "quuid.h"
#include <QPdfWriter>
#include <QPainter>
#include <QImage>
#include <QFileInfo>
#include <QProcess>
#include <QDir>
#include <QMimeDatabase>
#include <QMimeType>





bool PopplerPdfProcessor::imagesToPdf(const QStringList &imageFiles, const QString &outputFile) {
    if (imageFiles.isEmpty()) return false;

    QPdfWriter writer(outputFile);
    writer.setPageLayout(QPageLayout(QPageSize(QPageSize::A4), QPageLayout::Portrait, QMarginsF()));
    writer.setResolution(900);

    QPainter painter(&writer);

    for (int i = 0; i < imageFiles.size(); ++i) {
        QImage img(imageFiles.at(i));
        if (img.isNull()) continue;

        if (i > 0) writer.newPage();

        QRect targetRect = writer.pageLayout().paintRectPixels(writer.resolution());

        QImage scaledImg = img.scaled(targetRect.size(),
                                      Qt::KeepAspectRatio,
                                      Qt::SmoothTransformation);

        int x = targetRect.left() + (targetRect.width() - scaledImg.width()) / 2;
        int y = targetRect.top() + (targetRect.height() - scaledImg.height()) / 2;

        painter.drawImage(x, y, scaledImg);
    }

    painter.end();
    return true;
}

bool PopplerPdfProcessor::renderPageToImage(const QString &sourceFile, int pageIndex, const QString &outputImagePath) {
    Poppler::Document* doc = Poppler::Document::load(sourceFile);
    if (!doc) return false;

    Poppler::Page* page = doc->page(pageIndex);
    if (!page) {
        delete doc;
        return false;
    }

    QImage img = page->renderToImage(150, 150);
    bool success = img.save(outputImagePath, "PNG");

    delete page;
    delete doc;
    return success;
}

int PopplerPdfProcessor::getPageCount(const QString &sourceFile) {
    Poppler::Document* doc = Poppler::Document::load(sourceFile);
    if (!doc) return 0;
    int count = doc->numPages();
    delete doc;
    return count;
}

bool PopplerPdfProcessor::extractPages(const QString &sourceFile, const QString &outputFile, const QVector<int> &pageIndices) {
    Poppler::Document* doc = Poppler::Document::load(sourceFile);
    if (!doc) return false;

    QPdfWriter writer(outputFile);
    QPainter painter(&writer);

    bool firstPage = true;
    for (int idx : pageIndices) {
        if (idx < 0 || idx >= doc->numPages()) continue;

        Poppler::Page* page = doc->page(idx);
        if (page) {
            if (!firstPage) writer.newPage();

            QImage img = page->renderToImage(300, 300);
            painter.drawImage(writer.pageLayout().paintRectPixels(writer.resolution()), img);

            delete page;
            firstPage = false;
        }
    }
    delete doc;
    return !firstPage;
}
