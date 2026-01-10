#ifndef PDFFILEMODEL_H
#define PDFFILEMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include "FileItem.h"

class PdfFileModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum FileRoles {
        FileNameRole = Qt::UserRole + 1,
        FilePathRole,
        FileSizeRole,
        PreviewRole,
        IsProcessingRole,
        IsErrorRole,
        ProgressRole
    };

    explicit PdfFileModel(QObject *parent = nullptr);

    // Базовые методы QAbstractListModel
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;

    // Методы управления списком
    void addFile(const QString &filePath);
    void removeFile(int index);
    void clearAll();
    void moveRow(int from, int to);

    // Вспомогательные методы
    QStringList getAllPaths() const;
    void updatePreview(const QString &path, const QImage &image);
    QImage getPreviewImage(int index) const;

private:
    QVector<FileItem> m_files;
    QString generateUniqueName(const QString &originalName);
    QString formatSize(qint64 bytes);
};

#endif // PDFFILEMODEL_H
