#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QStringList>
#include <QFutureWatcher>
#include <QVariantList>

struct FileContent {
    QString path;
    QString content;
    QString processedContent;
};

class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool processing READ isProcessing NOTIFY processingChanged)

public:
    explicit Backend(QObject *parent = nullptr);

    bool isProcessing() const;
    Q_INVOKABLE void processFiles(const QStringList &filePaths);
    Q_INVOKABLE void cancelProcessing();
    Q_INVOKABLE QString getProcessedContent(const QString &filePath);

signals:
    void processingChanged(bool processing);
    void comparisonFinished(double similarityScore, const QVariantList &matches);
    void errorOccurred(const QString &message);

private slots:
    void setProcessing(bool processing);

private:
    QString loadAndPreprocess(const QString &filePath);
    void compareAllFiles();

    bool m_isProcessing = false;
    QFutureWatcher<void> m_watcher;
    QList<FileContent> m_loadedFiles;
    QHash<QString, QString> m_processedCache;
};

#endif // BACKEND_H
