#ifndef IMAGETOPDFTASK_H
#define IMAGETOPDFTASK_H

#include "BaseTask.h"
#include "QtImageConverter.h"
#include <QStringList>
#include <QFileInfo>

class ImageToPdfTask : public BaseTask {
public:
    ImageToPdfTask(const QStringList &imagePaths, const QString &outputDir)
        : m_imagePaths(imagePaths), m_outputDir(outputDir) {}

    void run() override {
        emit m_signals->started();

        QtImageConverter converter;
        int total = m_imagePaths.size();

        for (int i = 0; i < total; ++i) {
            QString imgPath = m_imagePaths.at(i);

            QString resultPath = converter.convertImageToPdf(imgPath, m_outputDir);

            if (resultPath.isEmpty()) {
                QFileInfo info(imgPath);
                emit m_signals->error("Не удалось конвертировать: " + info.fileName());
                return;
            }

            int percentage = static_cast<int>((static_cast<double>(i + 1) / total) * 100);
            emit m_signals->progress(percentage);
        }

        emit m_signals->finished(m_outputDir);
    }

private:
    QStringList m_imagePaths;
    QString m_outputDir;
};

#endif // IMAGETOPDFTASK_H
