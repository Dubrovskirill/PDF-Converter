#include <QtTest>
#include <QDir>
#include <QFile>
#include <QImage>
#include "QtPdfRenderer.h"

class TestPdfRenderer : public QObject
{
    Q_OBJECT

private slots:
    // Подготовка: создаем чистую папку для результатов рендеринга
    void initTestCase() {
        QDir dir(m_baseOutputPath);
        if (dir.exists()) dir.removeRecursively();
        QDir().mkpath(m_baseOutputPath);
    }

    // 1. Тест: Рендеринг PDF (проверка Poppler)
    void test_renderPdf() {
        QtPdfRenderer renderer;
        QString testFile = m_testFilesPath + "Tri-fold.pdf";
        QString outputFile = m_baseOutputPath + "pdf_preview.png";

        // Проверяем наличие исходника
        QVERIFY2(QFile::exists(testFile), "Тестовый PDF не найден");

        // Рендерим первую страницу в размере 300x300
        QImage result = renderer.renderPreview(testFile, 0, QSize(300, 300));

        // Проверки
        QVERIFY2(!result.isNull(), "Рендер PDF вернул пустую картинку");
        QVERIFY(result.save(outputFile));

        qDebug() << "PDF render saved to:" << outputFile;
    }

    // 2. Тест: Рендеринг обычного изображения (проверка QImageReader)
    void test_renderImage() {
        QtPdfRenderer renderer;
        QString testFile = m_testFilesPath + "desenhos.jpg";
        QString outputFile = m_baseOutputPath + "image_preview.png";

        QVERIFY2(QFile::exists(testFile), "Тестовое изображение не найдено");

        // Рендерим картинку, заставляя её вписаться в 200x200
        QImage result = renderer.renderPreview(testFile, 0, QSize(200, 200));

        QVERIFY2(!result.isNull(), "Рендер изображения вернул пустую картинку");
        QCOMPARE(qMax(result.width(), result.height()), 200);
        QVERIFY(result.save(outputFile));

        qDebug() << "Image render saved to:" << outputFile;
    }

    // 3. Тест: Проверка количества страниц
    void test_getPageCount() {
        QtPdfRenderer renderer;
        QString testFile = m_testFilesPath + "Tri-fold.pdf";

        int pages = renderer.getPageCount(testFile);

        QVERIFY(pages > 0);
        qDebug() << "Total pages in PDF:" << pages;
    }

private:
    QString m_testFilesPath = "D:/c++ everything/PDF-Converter/test_files/";
    QString m_baseOutputPath = "D:/c++ everything/PDF-Converter/test_output/render_results/";
};

//QTEST_MAIN(TestPdfRenderer)
//#include "TestPdfRenderer.moc"
