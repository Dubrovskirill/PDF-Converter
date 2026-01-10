#include "PreviewImageProvider.h"

PreviewImageProvider::PreviewImageProvider(PdfFileModel *model)
    : QQuickImageProvider(QQuickImageProvider::Image)
    , m_model(model)
{
}

QImage PreviewImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    bool ok;
    int index = id.toInt(&ok);

    if (!ok || !m_model) {
        return QImage();
    }


    QImage img = m_model->getPreviewImage(index);

    if (img.isNull()) {

        QImage empty(requestedSize.isValid() ? requestedSize : QSize(200, 200), QImage::Format_ARGB32);
        empty.fill(Qt::transparent);
        return empty;
    }

    if (size) *size = img.size();

    if (requestedSize.isValid()) {
        return img.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    return img;
}
