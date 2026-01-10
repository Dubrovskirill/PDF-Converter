#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "ConverterViewModel.h"
#include "PreviewImageProvider.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // 1. Создаем экземпляр нашей ViewModel.
    // Она будет жить на протяжении всего времени работы приложения.
    ConverterViewModel converterVM;

    // 2. Регистрация Image Provider.
    // Теперь QML будет знать, что при запросе ссылки "image://previews/..."
    // нужно обращаться к нашему классу.
    engine.addImageProvider(QLatin1String("previews"), new PreviewImageProvider(converterVM.fileModel()));

    // 3. Регистрация ViewModel в контексте QML.
    // Это позволит вам писать в коде QML просто "converterVM.addFiles(...)"
    engine.rootContext()->setContextProperty("converterVM", &converterVM);

    // 4. Регистрация Модели.
    // Удобно иметь прямой доступ к модели для GridView/ListView
    engine.rootContext()->setContextProperty("fileModel", converterVM.fileModel());

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
