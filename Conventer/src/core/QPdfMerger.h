#ifndef QPDFMERGER_H
#define QPDFMERGER_H

#include "interfaces/IPdfMerger.h"
#include "QtImageConverter.h"
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QFile>

// Подключаем QPDF
#include <qpdf/QPDF.hh>
#include <qpdf/QPDFWriter.hh>

class QPdfMerger : public IPdfMerger {
public:
    bool mergeFiles(const QStringList &sourceFiles, const QString &outputFilePath) override {
        if (sourceFiles.isEmpty()) return false;

        QStringList tempFiles;
        QtImageConverter imageConverter;
        bool success = false;

        try {
            // ПРОВЕРКА 1: Создаем папку, если её нет
            QFileInfo outFileInfo(outputFilePath);
            QDir targetDir = outFileInfo.dir();
            if (!targetDir.exists()) {
                if (!targetDir.mkpath(".")) {
                    qCritical() << "КРИТИЧЕСКАЯ ОШИБКА: Не удалось создать папку" << targetDir.path();
                    return false;
                }
            }

            // ПРОВЕРКА 2: Тестовое открытие файла средствами Qt (проверка на кириллицу/права доступа)
            QFile testFile(outputFilePath);
            if (!testFile.open(QIODevice::WriteOnly)) {
                qCritical() << "ОШИБКА ДОСТУПА: Система не дает создать файл:" << outputFilePath;
                qCritical() << "Причина:" << testFile.errorString();
                                                    return false;
            }
            testFile.close();
            testFile.remove(); // Удаляем пустышку перед работой QPDF

            QPDF combinedPdf;
            combinedPdf.emptyPDF();

            for (const QString &filePath : sourceFiles) {
                QString pdfToProcess;
                QFileInfo fileInfo(filePath);
                QString ext = fileInfo.suffix().toLower();

                if (ext == "pdf") {
                    pdfToProcess = filePath;
                } else if (ext == "jpg" || ext == "jpeg" || ext == "png") {
                    pdfToProcess = imageConverter.convertImageToPdf(filePath, QDir::tempPath());
                    if (!pdfToProcess.isEmpty()) {
                        tempFiles << pdfToProcess;
                    }
                }

                if (!pdfToProcess.isEmpty() && QFile::exists(pdfToProcess)) {
                    QPDF inputPdf;
                    // Используем Utf8 для чтения
                    inputPdf.processFile(pdfToProcess.toUtf8().constData());

                    std::vector<QPDFObjectHandle> pages = inputPdf.getAllPages();
                    for (auto& page : pages) {
                        combinedPdf.addPage(page, false);
                    }
                } else {
                    qWarning() << "Файл пропущен или не найден:" << filePath;
                }
            }

            // ЗАПИСЬ: Используем Utf8 (заменили toLocal8Bit на toUtf8)
            QPDFWriter writer(combinedPdf, outputFilePath.toUtf8().constData());
            writer.setStaticID(true);
            writer.write();

            // ПРОВЕРКА 3: Финальная проверка, что файл физически появился
            if (QFile::exists(outputFilePath)) {
                success = true;
                qDebug() << "УСПЕХ: Файл создан и проверен:" << outputFilePath;
            } else {
                qCritical() << "ФАНТОМНАЯ ОШИБКА: QPDF не записал файл по пути:" << outputFilePath;
                success = false;
            }

        } catch (std::exception &e) {
            qCritical() << "Исключение QPDF при слиянии:" << e.what();
            success = false;
        }

        cleanup(tempFiles);
        return success;
    }

private:
    void cleanup(const QStringList &files) {
        for (const QString &file : files) {
            if (QFile::exists(file)) {
                QFile::remove(file);
            }
        }
    }
};

#endif // QPDFMERGER_H
