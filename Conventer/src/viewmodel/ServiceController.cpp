#include "ServiceController.h"
#include "TaskQueueManager.h"
#include "RenderTask.h"
#include <QFileInfo>
#include <QUrl>
#include "ImageToPdfTask.h"
#include "MergeTask.h"

ServiceController::ServiceController(QObject *parent) : QObject(parent) {
    m_filesModel = new FilesModel(this);
    m_previewProvider = new PreviewProvider();

    m_outputDir = QDir::homePath() + "/PDF_Converter_Output";
    QDir().mkpath(m_outputDir);
}

QString ServiceController::generateUniquePath(const QString &fullPath) {
    QFileInfo info(fullPath);
    QString dir = info.absolutePath();
    QString base = info.completeBaseName();
    QString ext = info.suffix();

    QString newPath = fullPath;
    int index = 2;

    while (QFile::exists(newPath)) {
        newPath = QString("%1/%2_%3.%4").arg(dir, base, QString::number(index++), ext);
    }
    return newPath;
}

void ServiceController::addFiles(const QStringList &urls) {
    for (const QString &urlStr : urls) {

        QString localPath = QUrl(urlStr).toLocalFile();
        if (localPath.isEmpty()) localPath = urlStr;

        QFileInfo info(localPath);

        FileItem item;
        item.filePath = localPath;
        item.fileName = info.fileName();
        item.fileSize = QString::number(info.size() / 1024) + " KB";
        item.isProcessing = true;

        m_filesModel->addFile(item);

        RenderTask *task = new RenderTask(localPath);

        connect(task->taskSignals(), &TaskSignals::resultReady, this, [this, localPath](const QImage &img){
            m_previewProvider->addPreview(localPath, img);
            m_filesModel->updatePreview(localPath, img);
        });

        TaskQueueManager::instance().startTask(task);
    }
}

void ServiceController::removeFile(int index) {
    m_filesModel->removeFile(index);
}

void ServiceController::runConversion(bool mergeToOne, const QString &serviceType) {
    QStringList paths = m_filesModel->getAllPaths();
    if (paths.isEmpty()) {
        emit conversionError("Список файлов пуст");
        return;
    }

    emit conversionStarted();

    if (serviceType == "imagesToPdf") {
        if (mergeToOne) {
            // Если нужно объединить картинки в один файл
            QString outPath = generateUniquePath(m_outputDir + "/Combined_Images.pdf");
            MergeTask *task = new MergeTask(paths, outPath);
            setupTaskConnections(task);
            TaskQueueManager::instance().startTask(task);
        } else {
            // Каждая картинка в свой PDF
            ImageToPdfTask *task = new ImageToPdfTask(paths, m_outputDir);
            setupTaskConnections(task);
            TaskQueueManager::instance().startTask(task);
        }
    }
    else if (serviceType == "allToPdf") {
        // Сервис объединения PDF
        QString outPath = generateUniquePath(m_outputDir + "/Merged_Document.pdf");
        MergeTask *task = new MergeTask(paths, outPath);
        setupTaskConnections(task);
        TaskQueueManager::instance().startTask(task);
    }
}

// Вспомогательный метод для подписки на сигналы задачи
void ServiceController::setupTaskConnections(BaseTask* task) {
    connect(task->taskSignals(), &TaskSignals::progress, this, [this](int p){
        emit conversionProgress(static_cast<double>(p) / 100.0);
    });

    connect(task->taskSignals(), &TaskSignals::finished, this, [this](const QString &res){
        emit conversionFinished(res);
        // Помечаем в модели, что обработка закончена
        m_filesModel->clear(); // Обычно после успешной конвертации список очищают
    });

    connect(task->taskSignals(), &TaskSignals::error, this, [this](const QString &msg){
        emit conversionError(msg);
    });
}
