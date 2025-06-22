#include "backend.h"
#include "Rabin_karp.h"
#include "Preprocessor.h"
#include <QFile>
#include <QTextStream>
#include <QtConcurrent/QtConcurrentRun>
#include <QDebug>
#include <QUrl>
#include <QFileInfo>

Backend::Backend(QObject *parent) : QObject(parent)
{
    connect(&m_watcher, &QFutureWatcher<void>::finished, this, [this]() {
        setProcessing(false);
    });
}

bool Backend::isProcessing() const
{
    return m_isProcessing;
}

void Backend::setProcessing(bool processing)
{
    if (m_isProcessing != processing) {
        m_isProcessing = processing;
        emit processingChanged(processing);
    }
}

QString Backend::getProcessedContent(const QString &filePath)
{
    // Check cache first
    if (m_processedCache.contains(filePath)) {
        return m_processedCache[filePath];
    }

    // Load and process the file
    QString content = loadAndPreprocess(filePath);
    if (!content.isEmpty()) {
        m_processedCache[filePath] = content;
    }
    return content;
}

void Backend::processFiles(const QStringList &filePaths)
{
    if (filePaths.size() < 2) {
        emit errorOccurred(tr("Please select at least two files"));
        return;
    }

    // Validate all files exist
    for (const auto &path : filePaths) {
        QString localPath = path;
        if (path.startsWith("file:///")) {
            localPath = QUrl(path).toLocalFile();
        }

        if (!QFile::exists(localPath)) {
            emit errorOccurred(tr("File does not exist: %1").arg(localPath));
            return;
        }
    }

    setProcessing(true);
    m_loadedFiles.clear();

    QFuture<void> future = QtConcurrent::run([this, filePaths]() {
        try {
            for (const auto &path : filePaths) {
                FileContent fc;
                fc.path = path;

                // Load original content
                QString localPath = path;
                if (path.startsWith("file:///")) {
                    localPath = QUrl(path).toLocalFile();
                }

                QFile file(localPath);
                if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    emit errorOccurred(tr("Failed to open file: %1").arg(localPath));
                    return;
                }

                QTextStream in(&file);
                in.setAutoDetectUnicode(true);
                fc.content = in.readAll();
                file.close();

                if (fc.content.isEmpty()) {
                    emit errorOccurred(tr("File is empty: %1").arg(localPath));
                    return;
                }

                // Process content
                try {
                    Preprocessor preprocessor;
                    std::string stdContent = fc.content.toStdString();
                    std::string processed = preprocessor.preprocess(stdContent);
                    fc.processedContent = QString::fromStdString(processed);

                    if (fc.processedContent.isEmpty()) {
                        emit errorOccurred(tr("Failed to process file: %1").arg(localPath));
                        return;
                    }
                } catch (const std::exception &e) {
                    emit errorOccurred(tr("Preprocessing error for %1: %2").arg(localPath, e.what()));
                    return;
                }

                m_loadedFiles.append(fc);

                // Cache the processed content
                m_processedCache[path] = fc.processedContent;
            }

            compareAllFiles();
        } catch (const std::exception &e) {
            emit errorOccurred(tr("Processing error: %1").arg(e.what()));
        } catch (...) {
            emit errorOccurred(tr("Unknown processing error occurred"));
        }
    });

    m_watcher.setFuture(future);
}

QString Backend::loadAndPreprocess(const QString &filePath)
{
    QString localPath = filePath;
    if (filePath.startsWith("file:///")) {
        localPath = QUrl(filePath).toLocalFile();
    }

    QFile file(localPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file:" << localPath;
        return "";
    }

    QTextStream in(&file);
    in.setAutoDetectUnicode(true);
    QString content = in.readAll();
    file.close();

    if (content.isEmpty()) {
        qWarning() << "Empty file:" << localPath;
        return "";
    }

    try {
        Preprocessor preprocessor;
        std::string stdContent = content.toStdString();
        std::string processed = preprocessor.preprocess(stdContent);
        return QString::fromStdString(processed);
    } catch (const std::exception &e) {
        qWarning() << "Preprocessing error:" << e.what();
        return "";
    }
}

void Backend::compareAllFiles()
{
    if (m_loadedFiles.size() < 2) {
        emit errorOccurred(tr("Not enough files loaded for comparison"));
        return;
    }

    RabinKarp rk;
    QVariantList matches;
    double totalScore = 0;
    int comparisons = 0;

    // Compare each pair of files using processed content
    for (int i = 0; i < m_loadedFiles.size(); ++i) {
        for (int j = i + 1; j < m_loadedFiles.size(); ++j) {
            const auto &file1 = m_loadedFiles[i];
            const auto &file2 = m_loadedFiles[j];

            if (file1.processedContent.isEmpty() || file2.processedContent.isEmpty()) {
                qWarning() << "Skipping comparison due to empty processed content";
                continue;
            }

            try {
                double similarity = rk.computeSimilarity(
                    file1.processedContent.toStdString(),
                    file2.processedContent.toStdString(),
                    5 // k-gram size
                    );

                totalScore += similarity;
                comparisons++;

                QVariantMap match;
                match["file1"] = file1.path;
                match["file2"] = file2.path;
                match["score"] = similarity * 100;

                // Get match positions for detailed view
                auto matchPositions = rk.findMatches(
                    file1.processedContent.toStdString(),
                    file2.processedContent.toStdString(),
                    5
                    );

                QVariantList segments;
                for (const auto &pos : matchPositions) {
                    QVariantMap segment;
                    segment["pos1"] = static_cast<int>(pos.first);
                    segment["pos2"] = static_cast<int>(pos.second);
                    segments.append(segment);
                }
                match["segments"] = segments;

                matches.append(match);
            } catch (const std::exception &e) {
                qWarning() << "Comparison error:" << e.what();
            }
        }
    }

    if (comparisons == 0) {
        emit errorOccurred(tr("No valid comparisons could be made"));
    } else {
        double averageScore = totalScore / comparisons;
        emit comparisonFinished(averageScore * 100, matches);
    }
}

void Backend::cancelProcessing()
{
    if (m_watcher.isRunning()) {
        m_watcher.cancel();
        setProcessing(false);
        emit errorOccurred(tr("Processing cancelled by user"));
    }
}
