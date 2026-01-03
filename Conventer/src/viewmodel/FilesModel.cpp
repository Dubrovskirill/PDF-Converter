#include "FilesModel.h"

FilesModel::FilesModel(QObject *parent) : QAbstractListModel(parent) {}

int FilesModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return m_files.count();
}

QHash<int, QByteArray> FilesModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[FileNameRole] = "fileName";
    roles[FileSizeRole] = "fileSize";
    roles[PreviewSourceRole] = "previewSource";
    roles[IsProcessingRole] = "isProcessing";
    roles[IsErrorRole] = "isError";
    return roles;
}

QVariant FilesModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_files.size())
        return QVariant();

    const FileItem &item = m_files.at(index.row());

    switch (role) {
    case FileNameRole: return item.fileName;
    case FileSizeRole: return item.fileSize;
    case PreviewSourceRole: return item.preview;
    case IsProcessingRole: return item.isProcessing;
    case IsErrorRole: return item.isError;
    default: return QVariant();
    }
}

void FilesModel::addFile(const FileItem &item) {
    beginInsertRows(QModelIndex(), m_files.size(), m_files.size());
    m_files.append(item);
    endInsertRows();
}

void FilesModel::removeFile(int index) {
    if (index < 0 || index >= m_files.size()) return;
    beginRemoveRows(QModelIndex(), index, index);
    m_files.removeAt(index);
    endRemoveRows();
}

void FilesModel::updatePreview(const QString &path, const QImage &image) {
    for (int i = 0; i < m_files.size(); ++i) {
        if (m_files[i].filePath == path) {
            m_files[i].preview = image;
            m_files[i].isProcessing = false;
            QModelIndex idx = index(i);
            emit dataChanged(idx, idx, {PreviewSourceRole, IsProcessingRole});
            break;
        }
    }
}

void FilesModel::updateStatus(const QString &path, bool processing, bool error) {
    for (int i = 0; i < m_files.size(); ++i) {
        if (m_files[i].filePath == path) {
            m_files[i].isProcessing = processing;
            m_files[i].isError = error;
            QModelIndex idx = index(i);
            emit dataChanged(idx, idx, {IsProcessingRole, IsErrorRole});
            break;
        }
    }
}

void FilesModel::clear() {
    beginResetModel();
    m_files.clear();
    endResetModel();
}

QStringList FilesModel::getAllPaths() const {
    QStringList paths;
    for (const auto &item : m_files) paths << item.filePath;
    return paths;
}
