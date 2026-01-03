#ifndef FILESMODEL_H
#define FILESMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include <QImage>
#include "FileItem.h"

class FilesModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum FileRoles {
        FileNameRole = Qt::UserRole + 1,
        FileSizeRole,
        PreviewSourceRole,
        IsProcessingRole,
        IsErrorRole
    };

    explicit FilesModel(QObject *parent = nullptr);

    // Базовые методы QAbstractListModel
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Исправленный метод (теперь возвращает QByteArray)
    QHash<int, QByteArray> roleNames() const override;

    // Методы управления данными (для ServiceController)
    void addFile(const FileItem &item);
    void removeFile(int index);
    void updatePreview(const QString &path, const QImage &image);
    void updateStatus(const QString &path, bool processing, bool error = false);
    void clear();

    QStringList getAllPaths() const;

private:
    QVector<FileItem> m_files;
};

#endif // FILESMODEL_H
