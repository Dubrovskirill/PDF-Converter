#ifndef TASKSIGNALS_H
#define TASKSIGNALS_H

#include <QObject>
#include <QString>


class TaskSignals : public QObject {
    Q_OBJECT
public:
    explicit TaskSignals(QObject *parent = nullptr) : QObject(parent) {}

signals:

    void finished(const QString &result);

    void error(const QString &message);

    void progress(int percentage);

    void started();

    void resultReady(const QImage &image);
};

#endif // TASKSIGNALS_H
