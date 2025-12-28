#ifndef IPDFPROCESSOR_H
#define IPDFPROCESSOR_H

#include <QString>
#include <QStringList>
#include <QVector>


class IPdfProcessor {
public:
    virtual ~IPdfProcessor() = default;


    virtual bool mergeFiles(const QStringList &sourceFiles, const QString &outputFile) = 0;
    virtual bool extractPages(const QString &sourceFile, const QString &outputFile, const QVector<int> &pageIndices) = 0;
    virtual bool renderPageToImage(const QString &sourceFile, int pageIndex, const QString &outputImagePath) = 0;
    virtual int getPageCount(const QString &sourceFile) = 0;
    virtual bool imagesToPdf(const QStringList &imageFiles, const QString &outputFile) = 0;
};

#endif // IPDFPROCESSOR_H
