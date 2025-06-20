#include "backend.h"
#include "Rabin_karp.h"
#include "Preprocessor.h"
#include <QFile>
#include <QTextStream>
#include <QtConcurrent/QtConcurrentRun>
#include <QDebug>

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

void Backend::processFiles(const QStringList &filePaths)
{
    if (filePaths.size() < 2) {
        emit errorOccurred(tr("Please select at least two files"));
        return;
    }

    for (const auto &path : filePaths) {
        if (!QFile::exists(path)) {
            emit errorOccurred(tr("File does not exist: %1").arg(path));
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
                fc.content = loadAndPreprocess(path);
                if (fc.content.isEmpty()) {
                    emit errorOccurred(tr("Failed to process file: %1").arg(path));
                    return;
                }
                m_loadedFiles.append(fc);
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
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file:" << filePath;
        return "";
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    if (content.isEmpty()) {
        qWarning() << "Empty file:" << filePath;
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

    // Compare each pair of files
    for (int i = 0; i < m_loadedFiles.size(); ++i) {
        for (int j = i + 1; j < m_loadedFiles.size(); ++j) {
            const auto &file1 = m_loadedFiles[i];
            const auto &file2 = m_loadedFiles[j];

            if (file1.content.isEmpty() || file2.content.isEmpty()) {
                qWarning() << "Skipping comparison due to empty content";
                continue;
            }

            try {
                double similarity = rk.computeSimilarity(
                    file1.content.toStdString(),
                    file2.content.toStdString(),
                    5 // k-gram size
                    );

                totalScore += similarity;
                comparisons++;

                QVariantMap match;
                match["file1"] = file1.path;
                match["file2"] = file2.path;
                match["score"] = similarity * 100;
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
