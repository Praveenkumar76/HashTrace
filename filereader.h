#ifndef FILEREADER_H
#define FILEREADER_H

#include <QObject>
#include <QString>

class FileReader : public QObject
{
    Q_OBJECT

public:
    explicit FileReader(QObject *parent = nullptr);

    Q_INVOKABLE QString readFile(const QString &fileUrl);
    Q_INVOKABLE QString getFileInfo(const QString &fileUrl);

signals:
    void errorOccurred(const QString &message);
};

#endif // FILEREADER_H
