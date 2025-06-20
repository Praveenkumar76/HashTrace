#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "backend.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<Backend>("com.company.backend", 1, 0, "Backend");

    QQmlApplicationEngine engine;
    engine.loadFromModule("plagiarism.detector", "Main");

    return app.exec();
}
