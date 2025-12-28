#include <QtTest>
#include <QDir>
#include "src/core/QtImageConverter.h" // Путь к твоей реализации

class TestQtImageConverter : public QObject
{
    Q_OBJECT

private slots:
    // Выполняется перед первым тестом
    void initTestCase() {
        QDir().mkpath(m_outputPath);
    }

    // Тестируем метод batchConvertToPdf
    void test_batchConvertToPdf() {
        QtImageConverter converter;

        // Используем прямые слеши для путей в Qt
        QString testFilesPath = "D:/c++ everything/PDF-Converter/test_files/";
        QStringList images;
        images << testFilesPath + "desenhos.jpg"
               << testFilesPath + "загрузка.jpg"
               << testFilesPath + "загрузка (1).jpg"
               << testFilesPath + "загрузка (2).jpg"
               << testFilesPath + "олега режет.png";

        // Запуск
        bool result = converter.batchConvertToPdf(images, m_outputPath);

        // Проверки
        QVERIFY2(result, "Метод вернул false, возможно пути к файлам неверны или нет прав на запись");

        QDir outDir(m_outputPath);
        int pdfCount = outDir.entryList(QStringList() << "*.pdf", QDir::Files).size();
        QCOMPARE(pdfCount, images.size());
    }

private:
    QString m_outputPath = "D:/c++ everything/PDF-Converter/test_output/";
};

QTEST_MAIN(TestQtImageConverter)
#include "TestQtImageConverter.moc"
