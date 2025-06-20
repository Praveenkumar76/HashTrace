// main.cpp
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include "backend.h"

int main(int argc, char *argv[])

{
    // Enable local file reading
    qputenv("QML_XHR_ALLOW_FILE_READ", "1");

    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/app-icon.png"));

    qmlRegisterType<Backend>("com.company.backend", 1, 0, "Backend");

    QQmlApplicationEngine engine;
    engine.loadFromModule("plagiarism.detector", "Main");

    return app.exec();
}
