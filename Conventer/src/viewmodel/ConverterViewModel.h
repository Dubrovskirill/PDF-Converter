#ifndef CONVERTERVIEWMODEL_H
#define CONVERTERVIEWMODEL_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QStringList>
#include "PdfFileModel.h"

class ConverterViewModel : public QObject
{
    Q_OBJECT

    // Свойства для ServiceFooter.qml и управления состоянием UI
    Q_PROPERTY(PdfFileModel* fileModel READ fileModel CONSTANT)
    Q_PROPERTY(float progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged)
    Q_PROPERTY(bool isProcessing READ isProcessing NOTIFY isProcessingChanged)
    Q_PROPERTY(bool isFinished READ isFinished NOTIFY isFinishedChanged)

public:
    explicit ConverterViewModel(QObject *parent = nullptr);

    // Геттеры для свойств
    PdfFileModel* fileModel() const;
    float progress() const;
    QString statusText() const;
    bool isProcessing() const;
    bool isFinished() const;

    // Публичные методы для вызова из QML (Q_INVOKABLE)

    // 1. Добавление и управление списком
    Q_INVOKABLE void addFiles(const QList<QUrl> &urls);
    Q_INVOKABLE void removeFile(int index);
    Q_INVOKABLE void moveFile(int from, int to);
    Q_INVOKABLE void clearList();

    // 2. Основное действие (Конвертация/Слияние)
    // serviceType: "imagesToPdf" или "allToPdf"
    // combineToOne: состояние чекбокса (для 1-го сервиса)
    Q_INVOKABLE void runConversion(const QString &serviceType, bool combineToOne);

    // 3. Работа с результатом
    Q_INVOKABLE void openResultFolder();

signals:
    void progressChanged();
    void statusTextChanged();
    void isProcessingChanged();
    void isFinishedChanged();
    void errorOccurred(const QString &message);

private:
    // Внутренние методы для запуска фоновых задач
    void startRendering(const QString &filePath);
    void finalizeOperation(const QString &resultPath);
    QString generateUniqueOutputPath(const QString &baseName);

    PdfFileModel *m_fileModel;

    float m_progress;
    QString m_statusText;
    bool m_isProcessing;
    bool m_isFinished;

    QString m_lastOutputDir; // Папка последнего сохранения
};

#endif // CONVERTERVIEWMODEL_H
