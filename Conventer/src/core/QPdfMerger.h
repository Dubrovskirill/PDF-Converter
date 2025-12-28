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
                    inputPdf.processFile(pdfToProcess.toStdString().c_str());

                    std::vector<QPDFObjectHandle> pages = inputPdf.getAllPages();
                    for (auto& page : pages) {
                        combinedPdf.addPage(page, false);
                    }
                } else {
                    qWarning() << "Файл пропущен или не найден:" << filePath;
                }
            }

            QPDFWriter writer(combinedPdf, outputFilePath.toLocal8Bit().constData());
            writer.setStaticID(true);
            writer.write();
            success = true;

        } catch (std::exception &e) {
            qCritical() << "Ошибка QPDF при слиянии:" << e.what();
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
