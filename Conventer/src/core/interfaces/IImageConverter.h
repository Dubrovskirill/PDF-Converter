
#ifndef IIMAGECONVERTER_H
#define IIMAGECONVERTER_H

#include <QString>
#include <QStringList>

class IImageConverter {
public:
    virtual ~IImageConverter() = default;
    virtual bool imagesToPdf(const QStringList &imageFiles, const QString &outputPdfPath) = 0;
};

#endif // IIMAGECONVERTER_H
