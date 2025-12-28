#ifndef RENDERTASK_H
#define RENDERTASK_H

#include "BaseTask.h"
#include "QtPdfRenderer.h"
#include <QImage>

class RenderTask : public BaseTask {
public:
    RenderTask(const QString &filePath, int pageIndex = 0, QSize targetSize = QSize(200, 200))
        : m_filePath(filePath), m_pageIndex(pageIndex), m_targetSize(targetSize) {}

    void run() override {
        emit m_signals->started();

        QtPdfRenderer renderer;
        QImage result = renderer.renderPreview(m_filePath, m_pageIndex, m_targetSize);

        if (!result.isNull()) {
            emit m_signals->resultReady(result);
            emit m_signals->finished(m_filePath);
        } else {
            emit m_signals->error("Ошибка рендеринга: " + m_filePath);
        }
    }

private:
    QString m_filePath;
    int m_pageIndex;
    QSize m_targetSize;
};

#endif // RENDERTASK_H
