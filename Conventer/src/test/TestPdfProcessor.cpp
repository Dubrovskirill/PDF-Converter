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

//    // Тест 2: Конвертация изображений
//    void testImagesToPdf() {
//        QStringList images;
//        images << m_testFilesPath + "desenhos.jpg"
//               << m_testFilesPath + "загрузка.jpg"
//               << m_testFilesPath + "загрузка (1).jpg"
//               << m_testFilesPath + "загрузка (2).jpg"
//               << m_testFilesPath + "олега режет.png";

//        for(const QString& imgPath : images) {
//            QVERIFY2(QFile::exists(imgPath), QString("Image not found: %1").arg(imgPath).toUtf8());
//        }

//        QString output = "converted_images.pdf";
//        QVERIFY(m_processor->imagesToPdf(images, output));
//        QVERIFY(QFile::exists(output));

//        QCOMPARE(m_processor->getPageCount(output), 5);
//        qDebug() << "[SUCCESS] Images to PDF Test: Created 5-page PDF";
//    }

    // Тест 3: Слияние PDF файлов
    void testMergePdfs() {
        // 1. Подготовка путей
        QStringList sourceFiles;
        sourceFiles << m_testFilesPath + "Tri-fold.pdf"
                    << m_testFilesPath + "desenhos.jpg"
                    << m_testFilesPath + "загрузка.jpg"
                    << m_testFilesPath + "загрузка (1).jpg"
                    << m_testFilesPath + "загрузка (2).jpg"
                    << m_testFilesPath + "олега режет.png"
                    << m_testFilesPath + "Богдан Кирилл Сергеевич.pdf";

        QString outputFile = m_testFilesPath + "merged_result.pdf";

        // Удаляем старый результат теста, если он есть
        if (QFile::exists(outputFile)) {
            QFile::remove(outputFile);
        }

        // 2. Подсчет ожидаемого количества страниц
        int expectedPageCount = 0;
        QMimeDatabase db;
        for (const QString &file : sourceFiles) {
            if (db.mimeTypeForFile(file).name() == "application/pdf") {
                expectedPageCount += m_processor->getPageCount(file);
            } else {
                // Каждое изображение становится одной страницей PDF
                expectedPageCount += 1;
            }
        }

        // 3. Выполнение операции
        bool success = m_processor->mergeFiles(sourceFiles, outputFile);

        // 4. Проверки (Assertions)
        QVERIFY2(success, "Метод mergeFiles вернул false");
        QVERIFY2(QFile::exists(outputFile), "Итоговый PDF файл не был создан");

        int resultPageCount = m_processor->getPageCount(outputFile);
        QCOMPARE(resultPageCount, expectedPageCount);

        // Дополнительная проверка на валидность файла (размер > 0)
        QVERIFY(QFileInfo(outputFile).size() > 0);

        qDebug() << "TestMergePdfs: Success. Total pages:" << resultPageCount;
    }

    // Тест 4: Рендеринг страницы
//    void testRenderPage() {
//        QString source = m_testFilesPath + "Tri-fold.pdf";
//        QString outputImg = "preview_page_0.png";


//        bool success = m_processor->renderPageToImage(source, 3, outputImg);

//        QVERIFY2(success, "Failed to render PDF page");
//        QVERIFY(QFile::exists(outputImg));
//        qDebug() << "[SUCCESS] Render Test: Preview created";
//    }

//    void cleanupTestCase() {
//        delete m_processor;
//        qDebug() << "--- Test Session Finished ---";
//    }

private:
    PopplerPdfProcessor* m_processor;
    QString m_testFilesPath;
};

QTEST_MAIN(TestPdfProcessor)
#include "TestPdfProcessor.moc"
