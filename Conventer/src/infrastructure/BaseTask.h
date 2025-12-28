#ifndef BASETASK_H
#define BASETASK_H

#include <QRunnable>
#include "TaskSignals.h"


class BaseTask : public QRunnable {
public:
    BaseTask() {

        setAutoDelete(true);


        m_signals = new TaskSignals();
    }

    virtual ~BaseTask() {
        m_signals->deleteLater();
    }

    TaskSignals* taskSignals() const { return m_signals; }

    void run() override = 0;

protected:
    TaskSignals* m_signals;
};

#endif // BASETASK_H
