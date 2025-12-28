#include <QtTest>
#include <QDir>
#include <QFile>
#include "src/core/QtImageConverter.h"

class TestQtImageConverter : public QObject
{
    Q_OBJECT

private slots:
    // Очищаем общую папку один раз
    void initTestCase() {
        QDir dir(m_basePath);
        if (dir.exists()) {
            dir.removeRecursively();
        }
    }

    void test_singleConversion() {
        QtImageConverter converter;
        QString testImg = "D:/c++ everything/PDF-Converter/test_files/desenhos.jpg";
        QString singleOut = m_basePath + "single/";

        QString resultPath = converter.convertImageToPdf(testImg, singleOut);

        QVERIFY(!resultPath.isEmpty());
        QVERIFY(QFile::exists(resultPath));
        QCOMPARE(QFileInfo(resultPath).fileName(), QString("desenhos.pdf"));

        qDebug() << "Одиночный файл сохранен в:" << resultPath;
    }

    void test_batchConvertToPdf() {
        QtImageConverter converter;
        QString testFilesPath = "D:/c++ everything/PDF-Converter/test_files/";
        QString batchOut = m_basePath + "batch/";

        QStringList images;
        images << testFilesPath + "загрузка.jpg"
               << testFilesPath + "загрузка (1).jpg"
               << testFilesPath + "загрузка (2).jpg"
               << testFilesPath + "олега режет.png";

        bool result = converter.batchConvertToPdf(images, batchOut);
        QVERIFY2(result, "Массовая конвертация не удалась");

        QDir outDir(batchOut);
        int pdfCount = outDir.entryList(QStringList() << "*.pdf", QDir::Files).size();

        // Теперь здесь ровно столько, сколько в списке, никакой путаницы
        QCOMPARE(pdfCount, images.size());
        qDebug() << "Пакетная конвертация завершена в:" << batchOut;
    }

private:
    QString m_basePath = "D:/c++ everything/PDF-Converter/test_output/";
};

//QTEST_MAIN(TestQtImageConverter)
//#include "TestQtImageConverter.moc"
