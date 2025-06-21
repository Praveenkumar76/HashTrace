#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <QQmlContext>
#include "backend.h"
#include "filereader.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    qputenv("QML_XHR_ALLOW_FILE_READ", "1");
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/app-icon.png"));

    // Create the engine first
    QQmlApplicationEngine engine;

    // Register Backend as a QML type
    qmlRegisterType<Backend>("com.company.backend", 1, 0, "Backend");

    // Register FileReader as both a context property AND a QML type
    FileReader fileReader;
    qmlRegisterType<FileReader>("com.company.filereader", 1, 0, "FileReader");
    engine.rootContext()->setContextProperty("fileReader", &fileReader);

    engine.loadFromModule("plagiarism.detector", "Main");
    return app.exec();
}
