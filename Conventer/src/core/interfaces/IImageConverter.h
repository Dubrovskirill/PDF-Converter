#ifndef IIMAGECONVERTER_H
#define IIMAGECONVERTER_H

#include <QString>
#include <QStringList>

class IImageConverter {
public:
    virtual ~IImageConverter() = default;
    virtual bool batchConvertToPdf(const QStringList &imageFiles, const QString &outputDir) = 0;
    virtual QString convertImageToPdf(const QString &imagePath, const QString &outputDir) = 0;
};

#endif // IIMAGECONVERTER_H
