#include "filereader.h"
#include <QFile>
#include <QTextStream>
#include <QUrl>
#include <QFileInfo>
#include <QMimeDatabase>

FileReader::FileReader(QObject *parent) : QObject(parent) {}

QString FileReader::readFile(const QString &fileUrl) {
    QString localPath = QUrl(fileUrl).toLocalFile();
    QFile file(localPath);

    if (!file.exists()) {
        emit errorOccurred("File not found");
        return "";
    }

    QFileInfo info(localPath);
    if (info.size() > 10 * 1024 * 1024) { // 10MB limit
        emit errorOccurred("File too large (>10MB)");
        return "";
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit errorOccurred("Failed to open: " + file.errorString());
        return "";
    }

    QTextStream in(&file);
    in.setAutoDetectUnicode(true);
    return in.readAll();
}

QString FileReader::getFileInfo(const QString &fileUrl) {
    QFileInfo info(QUrl(fileUrl).toLocalFile());
    return QString("%1 | %2 KB | %3")
        .arg(info.suffix().toUpper())
        .arg(info.size() / 1024)
        .arg(info.lastModified().toString("yyyy-MM-dd"));
}
