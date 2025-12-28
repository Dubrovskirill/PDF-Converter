#include <QtTest>
#include <QSignalSpy>
#include <QDir>
#include "TaskQueueManager.h"
#include "MergeTask.h"
#include "ImageToPdfTask.h"
#include "RenderTask.h"

class TestAsyncTasks : public QObject {
    Q_OBJECT

private:
    QString m_basePath = "D:/c++ everything/PDF-Converter/test_files";
    // Указываем путь согласно твоему пожеланию
    QString m_outputPath = "D:/c++ everything/PDF-Converter/test_output/async_tests";

    void prepareDir(const QString &path) {
        QDir dir(path);
        if (dir.exists()) dir.removeRecursively();
        dir.mkpath(".");
    }

private slots:
    void initTestCase() {
        QDir().mkpath(m_outputPath);
    }

    void test_stressRenderWithSaving() {
        QString subDir = m_outputPath + "/render_results";
        prepareDir(subDir);

        QDir dir(m_basePath);
        QFileInfoList files = dir.entryInfoList({"*.pdf", "*.png", "*.jpg"}, QDir::Files);
        int totalFiles = files.size();
        int finishedCount = 0;

        for (const QFileInfo &file : files) {
            RenderTask* task = new RenderTask(file.absoluteFilePath(), 0, QSize(400, 400));

            // Сохраняем результат рендера на диск
            connect(task->taskSignals(), &TaskSignals::resultReady, this, [subDir, file](const QImage &img){
                img.save(subDir + "/" + file.completeBaseName() + "_preview.png");
            });

            connect(task->taskSignals(), &TaskSignals::finished, this, [&](){
                finishedCount++;
            });

            TaskQueueManager::instance().startTask(task);
        }

        QTRY_VERIFY_WITH_TIMEOUT(finishedCount == totalFiles, 40000);
        qDebug() << "Рендер завершен. Проверь папку:" << subDir;
    }

    void test_multiMergeBatch() {
        QString subDir = m_outputPath + "/multi_merge";
        prepareDir(subDir);

        QDir dir(m_basePath);
        QStringList allPdfs = dir.entryList({"*.pdf"}, QDir::Files);

        int batchSize = 5;
        int taskStarted = 0;
        int taskFinished = 0;

        for (int i = 0; i < allPdfs.size(); i += batchSize) {
            QStringList batch;
            for (int j = i; j < i + batchSize && j < allPdfs.size(); ++j) {
                batch << dir.absoluteFilePath(allPdfs[j]);
            }

            QString outName = QString("%1/batch_%2.pdf").arg(subDir).arg(i / batchSize);
            MergeTask* task = new MergeTask(batch, outName);

            taskStarted++;
            connect(task->taskSignals(), &TaskSignals::finished, this, [&](){
                taskFinished++;
            });

            TaskQueueManager::instance().startTask(task);
        }

        // Ждем выполнения всех пачек
        QTRY_VERIFY_WITH_TIMEOUT(taskFinished == taskStarted, 30000);
        qDebug() << "Создано небольших PDF архивов:" << taskFinished;
    }

    // Оставим конвертацию как была, она отработала хорошо
    void test_batchImageConversion() {
        QString subDir = m_outputPath + "/image_conversions";
        prepareDir(subDir);
        QDir dir(m_basePath);
        QStringList images;
        for(auto f : dir.entryList({"*.png", "*.jpg"}, QDir::Files))
            images << dir.absoluteFilePath(f);

        ImageToPdfTask* task = new ImageToPdfTask(images, subDir);
        QSignalSpy spy(task->taskSignals(), SIGNAL(finished(QString)));
        TaskQueueManager::instance().startTask(task);
        QVERIFY(spy.wait(60000));
    }
};

//QTEST_MAIN(TestAsyncTasks)
//#include "TestAsyncTasks.moc"
