// backend.h
#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QStringList>
#include <QVector>
#include <QFutureWatcher>
#include <QVariant>  // Add this
#include <QVariantList>

class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isProcessing READ isProcessing NOTIFY processingChanged)

public:
    explicit Backend(QObject *parent = nullptr);
    bool isProcessing() const;

public slots:
    void processFiles(const QStringList &filePaths);
    void cancelProcessing();

signals:
    void comparisonFinished(double similarityScore, const QVariantList &matches);
    void processingChanged(bool processing);
    void errorOccurred(const QString &message);

private:
    struct FileContent {
        QString path;
        QString content;
    };

    QFutureWatcher<void> m_watcher;
    bool m_isProcessing = false;
    QVector<FileContent> m_loadedFiles;

    void setProcessing(bool processing);
    QString loadAndPreprocess(const QString &filePath);
    void compareAllFiles();
};

#endif // BACKEND_H
