#ifndef SERVICECONTROLLER_H
#define SERVICECONTROLLER_H

#include <QObject>
#include <QDir>
#include "FilesModel.h"
#include "PreviewProvider.h"
#include "BaseTask.h"

class ServiceController : public QObject {
    Q_OBJECT
    Q_PROPERTY(FilesModel* filesModel READ filesModel CONSTANT)

public:
    explicit ServiceController(QObject *parent = nullptr);

    FilesModel* filesModel() const { return m_filesModel; }
    PreviewProvider* previewProvider() const { return m_previewProvider; }


    Q_INVOKABLE void addFiles(const QStringList &urls);
    Q_INVOKABLE void removeFile(int index);
    Q_INVOKABLE void runConversion(bool mergeToOne, const QString &serviceType);

signals:
    void conversionStarted();
    void conversionProgress(double progress);
    void conversionFinished(const QString &resultPath);
    void conversionError(const QString &message);

private:

    QString generateUniquePath(const QString &fullPath);

    void setupTaskConnections(BaseTask* task);

    void onPreviewReady(const QImage &image, const QString &originalPath);

    FilesModel *m_filesModel;
    PreviewProvider *m_previewProvider;
    QString m_outputDir;
};

#endif
