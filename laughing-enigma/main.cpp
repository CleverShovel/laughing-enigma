#include "signalshandler.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

int main(int argc, char* argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);

  app.setOrganizationName("Some Company");
  app.setOrganizationDomain("somecompany.com");
  app.setApplicationName("Amazing Application");

  QQuickStyle::setStyle("Material");

  QQmlApplicationEngine engine;

  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
  &app, [url](QObject * obj, const QUrl & objUrl) {
    if (!obj && url == objUrl)
      QCoreApplication::exit(-1);
  }, Qt::QueuedConnection);
  engine.load(url);

  QObject* root = engine.rootObjects()[0];

  auto* handlerSignals = new SignalsHandler(root);

  QObject::connect(root, SIGNAL(openBookSignal(QString)),
                   handlerSignals, SLOT(OpenBook(QString)));

  return app.exec();
}
