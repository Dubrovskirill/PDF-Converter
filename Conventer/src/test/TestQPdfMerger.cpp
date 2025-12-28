#include <QtTest>
#include <QDir>
#include <QFile>
#include "src/core/QPdfMerger.h"

class TestQPdfMerger : public QObject
{
    Q_OBJECT

private slots:
    // Подготовка: очищаем старые результаты перед тестом
    void initTestCase() {
        QDir dir(m_baseOutputPath);
        if (dir.exists()) dir.removeRecursively();
        QDir().mkpath(m_baseOutputPath);
    }

    // 1. Тест: Слияние ТОЛЬКО PDF
    void test_mergeOnlyPdf() {
        QPdfMerger merger;
        QString outDir = m_baseOutputPath + "only_pdf/";
        QDir().mkpath(outDir);
        QString resultFile = outDir + "merged_pdfs.pdf";

        QStringList files;
        files << m_testFilesPath + "Tri-fold.pdf"
              << m_testFilesPath + "Богдан Кирилл Сергеевич.pdf";

        bool result = merger.mergeFiles(files, resultFile);

        QVERIFY2(result, "Не удалось объединить PDF файлы");
        QVERIFY(QFile::exists(resultFile));
    }

    // 2. Тест: Слияние ТОЛЬКО изображений
    void test_mergeOnlyImages() {
        QPdfMerger merger;
        QString outDir = m_baseOutputPath + "only_images/";
        QDir().mkpath(outDir);
        QString resultFile = outDir + "merged_images.pdf";

        QStringList files;
        files << m_testFilesPath + "desenhos.jpg"
              << m_testFilesPath + "загрузка.jpg"
              << m_testFilesPath + "олега режет.png";

        bool result = merger.mergeFiles(files, resultFile);

        QVERIFY2(result, "Не удалось объединить изображения");
        QVERIFY(QFile::exists(resultFile));
    }

    // 3. Тест: Смешанное слияние (PDF + Картинки)
    void test_mergeMixed() {
        QPdfMerger merger;
        QString outDir = m_baseOutputPath + "mixed/";
        QDir().mkpath(outDir);
        QString resultFile = outDir + "merged_mixed.pdf";

        QStringList files;
        files << m_testFilesPath + "загрузка (1).jpg"
              << m_testFilesPath + "Tri-fold.pdf"
              << m_testFilesPath + "загрузка (2).jpg"
              << m_testFilesPath + "Богдан Кирилл Сергеевич.pdf";

        bool result = merger.mergeFiles(files, resultFile);

        QVERIFY2(result, "Не удалось объединить смешанные файлы");
        QVERIFY(QFile::exists(resultFile));
    }

private:
    QString m_testFilesPath = "D:/c++ everything/PDF-Converter/test_files/";
    QString m_baseOutputPath = "D:/c++ everything/PDF-Converter/test_output/merger_results/";
};

QTEST_MAIN(TestQPdfMerger)
#include "TestQPdfMerger.moc"
