#include "PdfFileModel.h"
#include <QFileInfo>
#include <QUrl>
#include <QDebug>

PdfFileModel::PdfFileModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int PdfFileModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return m_files.count();
}

QVariant PdfFileModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_files.count())
        return QVariant();

    const auto &item = m_files.at(index.row());

    switch (role) {
    case FileNameRole:
        return item.fileName;
    case FilePathRole:
        return item.filePath;
    case FileSizeRole:
        return item.fileSize;
    case PreviewRole:
        // Формируем URL для ImageProvider.
        // Добавляем флаг времени или индекс, чтобы QML обновлял картинку при изменении
        return QString("image://previews/%1").arg(index.row());
    case IsProcessingRole:
        return item.isProcessing;
    case IsErrorRole:
        return item.isError;
    case ProgressRole:
        return item.progress;
    default:
        return QVariant();
    }
}

bool PdfFileModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || index.row() >= m_files.count())
        return false;

    auto &item = m_files[index.row()];
    bool changed = false;

    switch (role) {
    case IsProcessingRole:
        if (item.isProcessing != value.toBool()) {
            item.isProcessing = value.toBool();
            changed = true;
        }
        break;
    case IsErrorRole:
        if (item.isError != value.toBool()) {
            item.isError = value.toBool();
            changed = true;
        }
        break;
    case ProgressRole:
        if (item.progress != value.toInt()) {
            item.progress = value.toInt();
            changed = true;
        }
        break;
    }

    if (changed) {
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

QHash<int, QByteArray> PdfFileModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[FileNameRole] = "fileName";
    roles[FilePathRole] = "filePath";
    roles[FileSizeRole] = "fileSize";
    roles[PreviewRole] = "previewSource";
    roles[IsProcessingRole] = "isProcessing";
    roles[IsErrorRole] = "isError";
    roles[ProgressRole] = "progress";
    return roles;
}

void PdfFileModel::addFile(const QString &filePath) {
    QFileInfo info(filePath);
    if (!info.exists()) return;

    beginInsertRows(QModelIndex(), m_files.count(), m_files.count());

    FileItem item;
    item.filePath = filePath;
    item.fileName = generateUniqueName(info.fileName());
    item.fileSize = formatSize(info.size());
    item.isProcessing = false;
    item.isError = false;
    item.progress = 0;

    m_files.append(item);
    endInsertRows();
}

void PdfFileModel::removeFile(int index) {
    if (index < 0 || index >= m_files.count()) return;

    beginRemoveRows(QModelIndex(), index, index);
    m_files.removeAt(index);
    endRemoveRows();
}

void PdfFileModel::clearAll() {
    if (m_files.isEmpty()) return;

    beginResetModel();
    m_files.clear();
    endResetModel();
}

void PdfFileModel::moveRow(int from, int to) {
    if (from < 0 || from >= m_files.count() || to < 0 || to >= m_files.count() || from == to)
        return;

    // beginMoveRows необходим для корректной анимации в QML (DelegateModel)
    int destination = (to > from) ? to + 1 : to;
    if (beginMoveRows(QModelIndex(), from, from, QModelIndex(), destination)) {
        m_files.move(from, to);
        endMoveRows();
    }
}

QStringList PdfFileModel::getAllPaths() const {
    QStringList paths;
    for (const auto &item : m_files) {
        paths << item.filePath;
    }
    return paths;
}

void PdfFileModel::updatePreview(const QString &path, const QImage &image) {
    for (int i = 0; i < m_files.count(); ++i) {
        if (m_files[i].filePath == path) {
            m_files[i].preview = image;
            QModelIndex idx = index(i);
            emit dataChanged(idx, idx, {PreviewRole});
            break;
        }
    }
}

QString PdfFileModel::generateUniqueName(const QString &originalName) {
    QFileInfo info(originalName);
    QString base = info.completeBaseName();
    QString suffix = info.suffix();
    QString name = originalName;
    int counter = 2;

    auto exists = [&](const QString &checkName) {
        for (const auto &f : m_files) {
            if (f.fileName == checkName) return true;
        }
        return false;
    };

    while (exists(name)) {
        name = QString("%1_%2.%3").arg(base).arg(counter++).arg(suffix);
    }
    return name;
}

QString PdfFileModel::formatSize(qint64 bytes) {
    double size = static_cast<double>(bytes);
    QString unit = "B";
    if (size > 1024) { size /= 1024; unit = "KB"; }
    if (size > 1024) { size /= 1024; unit = "MB"; }
    if (size > 1024) { size /= 1024; unit = "GB"; }
    return QString::number(size, 'f', 1) + " " + unit;
}

QImage PdfFileModel::getPreviewImage(int index) const {
    if (index < 0 || index >= m_files.count()) {
        return QImage();
    }
    return m_files.at(index).preview;
}
