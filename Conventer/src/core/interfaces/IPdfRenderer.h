#ifndef IPDFRENDERER_H
#define IPDFRENDERER_H

#include <QImage>
#include <QString>
#include <QSize>

class IPdfRenderer {
public:
    virtual ~IPdfRenderer() = default;

    virtual QImage renderPreview(const QString &filePath, int pageIndex = 0, QSize targetSize = QSize(200, 200)) = 0;

    virtual int getPageCount(const QString &filePath) = 0;
};

#endif // IPDFRENDERER_H
