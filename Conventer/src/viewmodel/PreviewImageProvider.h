#ifndef PREVIEWIMAGEPROVIDER_H
#define PREVIEWIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include "PdfFileModel.h"


class PreviewImageProvider : public QQuickImageProvider
{
public:
    explicit PreviewImageProvider(PdfFileModel *model);


    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

private:
    PdfFileModel *m_model;
};

#endif // PREVIEWIMAGEPROVIDER_H
