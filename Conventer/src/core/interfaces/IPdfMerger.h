#ifndef IPDFMERGER_H
#define IPDFMERGER_H

#include <QString>
#include <QStringList>


class IPdfMerger {
public:
    virtual ~IPdfMerger() = default;

    virtual bool mergeFiles(const QStringList &sourceFiles, const QString &outputFilePath) = 0;
};

#endif // IPDFMERGER_H
