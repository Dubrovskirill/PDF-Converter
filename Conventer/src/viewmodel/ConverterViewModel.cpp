#include "ConverterViewModel.h"
#include "TaskQueueManager.h"
#include "RenderTask.h"
#include "MergeTask.h"
#include "ImageToPdfTask.h"

#include <QStandardPaths>
#include <QDir>
#include <QDesktopServices>
#include <QDebug>

ConverterViewModel::ConverterViewModel(QObject *parent)
    : QObject(parent)
    , m_fileModel(new PdfFileModel(this))
    , m_progress(0)
    , m_statusText("Готов к работе")
    , m_isProcessing(false)
    , m_isFinished(false)
{
    // Определяем папку для сохранения (Документы/PDF_Converter)
    m_lastOutputDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/PDF_Converter";
}

// --- Геттеры ---
PdfFileModel* ConverterViewModel::fileModel() const { return m_fileModel; }
float ConverterViewModel::progress() const { return m_progress; }
QString ConverterViewModel::statusText() const { return m_statusText; }
bool ConverterViewModel::isProcessing() const { return m_isProcessing; }
bool ConverterViewModel::isFinished() const { return m_isFinished; }

// --- Взаимодействие со списком ---
void ConverterViewModel::addFiles(const QList<QUrl> &urls) {
    if (urls.isEmpty()) return;

    for (const QUrl &url : urls) {
        QString path = url.toLocalFile();
        if (path.isEmpty()) path = url.toString(); // Обработка путей в зависимости от ОС

        m_fileModel->addFile(path);
        startRendering(path); // Запускаем создание превью сразу при добавлении
    }

    m_isFinished = false;
    emit isFinishedChanged();
    m_statusText = QString("Добавлено файлов: %1").arg(m_fileModel->rowCount());
    emit statusTextChanged();
}

void ConverterViewModel::removeFile(int index) {
    m_fileModel->removeFile(index);
}

void ConverterViewModel::moveFile(int from, int to) {
    m_fileModel->moveRow(from, to);
}

void ConverterViewModel::clearList() {
    m_fileModel->clearAll();
    m_isFinished = false;
    m_progress = 0;
    m_isProcessing = false;
    emit isFinishedChanged();
    emit progressChanged();
    emit isProcessingChanged();
}

// --- Основная логика конвертации ---
void ConverterViewModel::runConversion(const QString &serviceType, bool combineToOne) {
    if (m_fileModel->rowCount() == 0) return;

    m_isProcessing = true;
    m_isFinished = false;
    m_progress = 0;
    m_statusText = "Запуск обработки...";
    emit isProcessingChanged();
    emit isFinishedChanged();
    emit statusTextChanged();

    QStringList sourceFiles = m_fileModel->getAllPaths();
    QDir().mkpath(m_lastOutputDir);

    if (serviceType == "imagesToPdf") {
        if (combineToOne) {
            // Режим: Все картинки в один PDF
            QString outPath = generateUniqueOutputPath("combined_images.pdf");
            MergeTask* task = new MergeTask(sourceFiles, outPath);
            connect(task->taskSignals(), &TaskSignals::progress, this, [this](int p){
                m_progress = p / 100.0f;
                emit progressChanged();
            });
            connect(task->taskSignals(), &TaskSignals::finished, this, &ConverterViewModel::finalizeOperation);
            TaskQueueManager::instance().startTask(task);
        } else {
            // Режим: Каждая картинка в отдельный PDF
            // Режим: каждая картинка в свой PDF
            QStringList targetPaths;
            for (const QString &srcPath : sourceFiles) {
                QFileInfo info(srcPath);
                // Генерируем уникальное имя для каждого выходного PDF
                QString outPath = generateUniqueOutputPath(info.completeBaseName() + ".pdf");
                targetPaths << outPath;
            }

            // Теперь передаем в задачу два списка: ЧТО конвертировать и КУДА сохранять
            ImageToPdfTask* task = new ImageToPdfTask(sourceFiles, targetPaths);

            connect(task->taskSignals(), &TaskSignals::progress, this, [this](int p){
                m_progress = p / 100.0f;
                emit progressChanged();
            });
            connect(task->taskSignals(), &TaskSignals::finished, this, &ConverterViewModel::finalizeOperation);
            TaskQueueManager::instance().startTask(task);
        }
    } else if (serviceType == "allToPdf") {
        // Режим: Объединение (PDF и картинки) в один файл
        QString outPath = generateUniqueOutputPath("merged_document.pdf");
        MergeTask* task = new MergeTask(sourceFiles, outPath);
        connect(task->taskSignals(), &TaskSignals::finished, this, &ConverterViewModel::finalizeOperation);
        TaskQueueManager::instance().startTask(task);
    }
}

// --- Вспомогательные методы ---
void ConverterViewModel::startRendering(const QString &filePath) {
    RenderTask* task = new RenderTask(filePath);
    // Когда превью готово, обновляем его в модели
    connect(task->taskSignals(), &TaskSignals::resultReady, this, [this, filePath](const QImage &img){
        m_fileModel->updatePreview(filePath, img);
    });
    TaskQueueManager::instance().startTask(task);
}

void ConverterViewModel::finalizeOperation(const QString &resultPath) {
    Q_UNUSED(resultPath); // Если вдруг мы решим не использовать переменную, это уберет warning

    m_isProcessing = false;
    m_isFinished = true;
    m_progress = 1.0;

    // Используем resultPath, чтобы показать пользователю, какой именно файл создан
    if (!resultPath.isEmpty()) {
        QFileInfo info(resultPath);
        if (info.isFile()) {
            m_statusText = "Готово! Создан файл: " + info.fileName();
        } else {
            m_statusText = "Завершено! Файлы сохранены в папку.";
        }
    } else {
        m_statusText = "Операция успешно завершена.";
    }

    emit isProcessingChanged();
    emit isFinishedChanged();
    emit progressChanged();
    emit statusTextChanged();
}

void ConverterViewModel::openResultFolder() {
    QDesktopServices::openUrl(QUrl::fromLocalFile(m_lastOutputDir));
}

QString ConverterViewModel::generateUniqueOutputPath(const QString &baseName) {
    QString fullPath = m_lastOutputDir + "/" + baseName;
    QFileInfo info(fullPath);

    if (!info.exists()) {
        return fullPath;
    }

    QString name = info.completeBaseName();
    QString suffix = info.suffix();
    int counter = 2;

    while (QFile::exists(fullPath)) {
        fullPath = QString("%1/%2_%3.%4")
                       .arg(m_lastOutputDir)
                       .arg(name)
                       .arg(counter++)
                       .arg(suffix);
    }
    return fullPath;
}
