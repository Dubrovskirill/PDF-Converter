#include <QtTest>
#include <QSignalSpy>
#include "TaskQueueManager.h"
#include "BaseTask.h"
#include <QThread>

// 1. Создаем тестовую задачу
class DummyTask : public BaseTask {
public:
    DummyTask(int ms = 500) : m_delay(ms) {}

    void run() override {
        // Проверяем, что мы в другом потоке
        if (QThread::currentThread() == qApp->thread()) {
            emit m_signals->error("Задача запущена в главном потоке!");
            return;
        }

        emit m_signals->started();

        // Имитируем работу
        QThread::msleep(m_delay / 2);
        emit m_signals->progress(50);

        QThread::msleep(m_delay / 2);
        emit m_signals->finished("Success");
    }

private:
    int m_delay;
};

// 2. Класс теста
class TestAsyncTasks : public QObject {
    Q_OBJECT

private slots:
    void test_taskExecutionFlow() {
        // Создаем задачу (она сама удалится благодаря autoDelete)
        DummyTask* task = new DummyTask(300);

        // Используем QSignalSpy для перехвата сигналов из другого потока
        QSignalSpy spyStarted(task->taskSignals(), SIGNAL(started()));
        QSignalSpy spyProgress(task->taskSignals(), SIGNAL(progress(int)));
        QSignalSpy spyFinished(task->taskSignals(), SIGNAL(finished(QString)));

        // Запускаем через наш менеджер
        TaskQueueManager::instance().startTask(task);

        // Ждем завершения (максимум 1 секунда), не блокируя цикл событий
        QVERIFY(spyFinished.wait(1000));

        // Проверяем последовательность
        QCOMPARE(spyStarted.count(), 1);
        QCOMPARE(spyProgress.count(), 1);
        QCOMPARE(spyProgress.at(0).at(0).toInt(), 50);
        QCOMPARE(spyFinished.count(), 1);
        QCOMPARE(spyFinished.at(0).at(0).toString(), QString("Success"));
    }

    void test_multipleTasks() {
        const int count = 5;
        QSignalSpy spyTotalFinished(&TaskQueueManager::instance(), nullptr); // Для примера

        for(int i = 0; i < count; ++i) {
            TaskQueueManager::instance().startTask(new DummyTask(100));
        }

        // Проверяем, что задачи ушли в работу
        QVERIFY(TaskQueueManager::instance().activeTasksCount() > 0);

        // Даем время всем завершиться
        QTest::qWait(1000);
        QCOMPARE(TaskQueueManager::instance().activeTasksCount(), 0);
    }
};

QTEST_MAIN(TestAsyncTasks)
#include "TestAsyncTasks.moc"
