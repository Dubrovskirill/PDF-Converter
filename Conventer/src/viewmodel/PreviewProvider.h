#ifndef PREVIEWPROVIDER_H
#define PREVIEWPROVIDER_H

#include <QQuickImageProvider>
#include <QCache>

class PreviewProvider : public QQuickImageProvider {
public:
    PreviewProvider() : QQuickImageProvider(QQuickImageProvider::Image) {}


    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override {
        if (m_cache.contains(id)) {
            QImage img = *m_cache.object(id);
            if (size) *size = img.size();
            return img;
        }
        return QImage();
    }

    void addPreview(const QString &id, const QImage &image) {

        m_cache.insert(id, new QImage(image));
    }

private:
    QCache<QString, QImage> m_cache;
};

#endif
