#ifndef POPPLERPDFPROCESSOR_H
#define POPPLERPDFPROCESSOR_H

#include "IPdfProcessor.h"
#include <poppler-qt5.h>

class PopplerPdfProcessor : public IPdfProcessor {
public:
    PopplerPdfProcessor() = default;
    ~PopplerPdfProcessor() override = default;

    bool mergeFiles(const QStringList &sourceFiles, const QString &outputFile) override;
    bool extractPages(const QString &sourceFile, const QString &outputFile, const QVector<int> &pageIndices) override;
    bool renderPageToImage(const QString &sourceFile, int pageIndex, const QString &outputImagePath) override;
    int getPageCount(const QString &sourceFile) override;

    bool imagesToPdf(const QStringList &imageFiles, const QString &outputFile) override;
};

#endif // POPPLERPDFPROCESSOR_H
