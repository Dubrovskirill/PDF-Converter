#ifndef MERGETASK_H
#define MERGETASK_H

#include "BaseTask.h"
#include "QPdfMerger.h"
#include <QStringList>


class MergeTask : public BaseTask {
public:
    MergeTask(const QStringList &srcFiles, const QString &destFile)
        : m_srcFiles(srcFiles), m_destFile(destFile) {}

    void run() override {
        emit m_signals->started();


        QPdfMerger merger;

        emit m_signals->progress(50);

        bool success = merger.mergeFiles(m_srcFiles, m_destFile);

        if (success) {
            emit m_signals->progress(100);
            emit m_signals->finished(m_destFile);
        } else {
            emit m_signals->error("Ошибка при объединении PDF файлов");
            emit m_signals->finished("");
        }
    }

private:
    QStringList m_srcFiles;
    QString m_destFile;
};

#endif // MERGETASK_H
