#ifndef SIGNALSHANDLER_H
#define SIGNALSHANDLER_H

#include <QObject>
#include <QString>
#include <QVariant>

class SignalsHandler : public QObject {
  Q_OBJECT
 public:
  explicit SignalsHandler(QObject* parent = nullptr);

 signals:

 public slots:
  void OpenBook(const QString& msg);
};

#endif // SIGNALSHANDLER_H
