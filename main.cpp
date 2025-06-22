#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <QDebug>
#include "backend.h"
#include "filereader.h"

int main(int argc, char *argv[]) {
    qputenv("QML_XHR_ALLOW_FILE_READ", "1");

    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon("qrc:/icons/app-icon.png"));

    // Register C++ types
    qmlRegisterType<Backend>("com.company.backend", 1, 0, "Backend");
    qmlRegisterType<FileReader>("com.company.filereader", 1, 0, "FileReader");

    QQmlApplicationEngine engine;

    // Debug output
    qDebug() << "QML import paths:" << engine.importPathList();

    // Load from QML module
    engine.loadFromModule("plagiarism.detector", "Main");

    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Failed to load QML interface";
        return -1;
    }

    return app.exec();
}
