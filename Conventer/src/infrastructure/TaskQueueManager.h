#ifndef TASKQUEUEMANAGER_H
#define TASKQUEUEMANAGER_H

#include <QObject>
#include <QThreadPool>
#include <QDebug>
#include "BaseTask.h"


class TaskQueueManager : public QObject {
    Q_OBJECT
public:
    static TaskQueueManager& instance() {
        static TaskQueueManager manager;
        return manager;
    }


    void startTask(BaseTask* task) {
        if (!task) {
            qWarning() << "TaskQueueManager: Попытка запустить пустую задачу!";
            return;
        }

        qDebug() << "TaskQueueManager: Задача поставлена в очередь. Свободно потоков:"
                 << m_pool->maxThreadCount() - m_pool->activeThreadCount();

        m_pool->start(task);
    }

    int activeTasksCount() const {
        return m_pool->activeThreadCount();
    }

private:
    explicit TaskQueueManager(QObject *parent = nullptr) : QObject(parent) {
        m_pool = QThreadPool::globalInstance();
        qDebug() << "TaskQueueManager: Инициализирован. Лимит потоков:" << m_pool->maxThreadCount();
    }

    TaskQueueManager(const TaskQueueManager&) = delete;
    TaskQueueManager& operator=(const TaskQueueManager&) = delete;

    QThreadPool* m_pool;
};

#endif // TASKQUEUEMANAGER_H
