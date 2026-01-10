#ifndef IMAGETOPDFTASK_H
#define IMAGETOPDFTASK_H

#include "BaseTask.h"
#include "QtImageConverter.h"
#include <QStringList>
#include <QFileInfo>

class ImageToPdfTask : public BaseTask {
public:
    // Теперь принимаем два списка: исходники и пути назначения
    ImageToPdfTask(const QStringList &imagePaths, const QStringList &targetPaths)
        : m_imagePaths(imagePaths), m_targetPaths(targetPaths) {}

    void run() override {
        emit m_signals->started();

        if (m_imagePaths.size() != m_targetPaths.size()) {
            emit m_signals->error("Ошибка: несоответствие количества путей.");
            return;
        }

        QtImageConverter converter;
        int total = m_imagePaths.size();

        for (int i = 0; i < total; ++i) {
            QString imgPath = m_imagePaths.at(i);
            QString outPath = m_targetPaths.at(i);


            bool success = converter.saveToPdf(imgPath, outPath);

            if (!success) {
                QFileInfo info(imgPath);
                emit m_signals->error("Не удалось конвертировать: " + info.fileName());
                return;
            }

            int percentage = static_cast<int>((static_cast<double>(i + 1) / total) * 100);
            emit m_signals->progress(percentage);
        }

        // Возвращаем путь к папке последнего созданного файла
        QString finalDir = total > 0 ? QFileInfo(m_targetPaths.last()).absolutePath() : QString();
        emit m_signals->finished(finalDir);
    }

private:
    QStringList m_imagePaths;
    QStringList m_targetPaths;
};

#endif // IMAGETOPDFTASK_H
