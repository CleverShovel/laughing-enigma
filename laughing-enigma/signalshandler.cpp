#include "signalshandler.h"
#include "fb2_extracter.h"

#include <QFile>
#include <QDebug>

SignalsHandler::SignalsHandler(QObject* parent) : QObject(parent) {

}

void SignalsHandler::OpenBook(const QString& msg) {
  auto* bookText = parent()->findChild<QObject*>("bookText");
  qDebug() << msg.mid(8);
  QFile file(msg.mid(8));
  file.open(QIODevice::ReadOnly);

  FB2Extracter extr(&file);
  bookText->setProperty("text", extr.GetBook());
  file.close();
}
