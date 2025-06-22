#include "filereader.h"
#include <QFile>
#include <QTextStream>
#include <QUrl>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QDebug>

FileReader::FileReader(QObject *parent) : QObject(parent) {}

QString FileReader::readFile(const QString &fileUrl) {
    QString localPath;

    // Handle both file:// URLs and direct paths
    if (fileUrl.startsWith("file:///")) {
        localPath = QUrl(fileUrl).toLocalFile();
    } else if (fileUrl.startsWith("file://")) {
        localPath = QUrl(fileUrl).toLocalFile();
    } else if (fileUrl.startsWith("/")) {
        localPath = fileUrl;
    } else {
        // Assume it's a relative path, convert to file URL first
        QUrl url = QUrl::fromLocalFile(fileUrl);
        localPath = url.toLocalFile();
    }

    QFile file(localPath);
    if (!file.exists()) {
        QString error = "File not found: " + localPath;
        emit errorOccurred(error);
        return "";
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString error = "Failed to open: " + file.errorString();
        emit errorOccurred(error);
        return "";
    }

    QTextStream in(&file);
    in.setAutoDetectUnicode(true);
    QString content = in.readAll();
    file.close();

    if (content.isEmpty()) {
        qWarning() << "Warning: File is empty:" << localPath;
    }

    return content;
}

QString FileReader::getFileInfo(const QString &fileUrl) {
    QString localPath;

    if (fileUrl.startsWith("file:///")) {
        localPath = QUrl(fileUrl).toLocalFile();
    } else if (fileUrl.startsWith("file://")) {
        localPath = QUrl(fileUrl).toLocalFile();
    } else {
        localPath = fileUrl;
    }

    QFileInfo info(localPath);
    if (!info.exists()) {
        return "File not found";
    }

    QString suffix = info.suffix().toUpper();
    if (suffix.isEmpty()) {
        suffix = "Unknown";
    }

    qint64 sizeKB = info.size() / 1024;
    if (sizeKB == 0 && info.size() > 0) {
        sizeKB = 1; // Show at least 1 KB for non-empty files
    }

    return QString("%1 | %2 KB | %3")
        .arg(suffix)
        .arg(sizeKB)
        .arg(info.lastModified().toString("yyyy-MM-dd"));
}
