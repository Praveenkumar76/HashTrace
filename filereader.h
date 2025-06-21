#ifndef FILEREADER_H
#define FILEREADER_H

#include <QObject>
#include <QString>
#include <QStringList>

class FileReader : public QObject
{
    Q_OBJECT
public:
    explicit FileReader(QObject *parent = nullptr);

    Q_INVOKABLE QString readFile(const QString &fileUrl);
    Q_INVOKABLE QString getFileInfo(const QString &fileUrl); // New: Get file size/type

signals:
    void errorOccurred(const QString &message); // For better error handling
};

#endif // FILEREADER_H
