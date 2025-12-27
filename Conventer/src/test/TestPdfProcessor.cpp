#include <QtTest>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include "PopplerPdfProcessor.h"

class TestPdfProcessor : public QObject {
    Q_OBJECT

private slots:
    void initTestCase() {
        m_processor = new PopplerPdfProcessor();
        m_testFilesPath = "D:\\c++ everything\\PDF-Converter\\test_files\\";

        qDebug() << "--- Test Session Started ---";
        qDebug() << "Working Directory:" << QDir::currentPath();
    }

    // Тест 1: Чтение существующего PDF
    void testGetPageCount() {
        QString path = m_testFilesPath + "Tri-fold.pdf";
        int count = m_processor->getPageCount(path);
        QVERIFY2(count > 0, QString("File not found or empty: %1").arg(path).toUtf8());
        qDebug() << "[SUCCESS] PDF Read Test: pages:" << count;
    }

    // Тест 2: Конвертация изображений
    void testImagesToPdf() {
        QStringList images;
        images << m_testFilesPath + "desenhos.jpg"
               << m_testFilesPath + "загрузка.jpg"
               << m_testFilesPath + "загрузка (1).jpg"
               << m_testFilesPath + "загрузка (2).jpg"
               << m_testFilesPath + "олега режет.png";

        for(const QString& imgPath : images) {
            QVERIFY2(QFile::exists(imgPath), QString("Image not found: %1").arg(imgPath).toUtf8());
        }

        QString output = "converted_images.pdf";
        QVERIFY(m_processor->imagesToPdf(images, output));
        QVERIFY(QFile::exists(output));

        QCOMPARE(m_processor->getPageCount(output), 5);
        qDebug() << "[SUCCESS] Images to PDF Test: Created 5-page PDF";
    }

    // Тест 3: Слияние PDF файлов (Merge)
    void testMergePdfs() {
        QStringList pdfs;
        pdfs << m_testFilesPath + "Tri-fold.pdf"
             << m_testFilesPath + "Богдан Кирилл Сергеевич.pdf";

        for(const QString& p : pdfs) {
            QVERIFY2(QFile::exists(p), QString("PDF not found for merge: %1").arg(p).toUtf8());
        }

        // Считаем сколько страниц должно быть в итоге
        int expectedPages = m_processor->getPageCount(pdfs[0]) + m_processor->getPageCount(pdfs[1]);

        QString output = "merged_result.pdf";
        bool success = m_processor->mergeFiles(pdfs, output);

        QVERIFY2(success, "Merge operation failed");
        QVERIFY(QFile::exists(output));
        QCOMPARE(m_processor->getPageCount(output), expectedPages);

        qDebug() << "[SUCCESS] Merge PDF Test: Created" << expectedPages << "page PDF";
    }

    // Тест 4: Рендеринг страницы
    void testRenderPage() {
        QString source = m_testFilesPath + "Tri-fold.pdf";
        QString outputImg = "preview_page_0.png";


        bool success = m_processor->renderPageToImage(source, 3, outputImg);

        QVERIFY2(success, "Failed to render PDF page");
        QVERIFY(QFile::exists(outputImg));
        qDebug() << "[SUCCESS] Render Test: Preview created";
    }

    void cleanupTestCase() {
        delete m_processor;
        qDebug() << "--- Test Session Finished ---";
    }

private:
    PopplerPdfProcessor* m_processor;
    QString m_testFilesPath;
};

QTEST_MAIN(TestPdfProcessor)
#include "TestPdfProcessor.moc"
