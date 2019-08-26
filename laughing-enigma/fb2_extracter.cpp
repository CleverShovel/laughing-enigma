#include "fb2_extracter.h"
#include "fmt/format.h"

#include <QDebug>

#include <string>
#include <unordered_map>
#include <variant>

using namespace std;

namespace FB2Parser {
Base::Base(QXmlStreamReader& xml) : xml(xml) {}

Document::Document(QXmlStreamReader& xml) : Base(xml) {}

void Document::Parse(QString& str) {
  str.reserve(str.size() + 50);
  str += "<!DOCTYPE HTML><html><body style=\"font-size:";
  str += to_string(font_size).c_str();
  str += "px\">";
}
} // FB2Parser


FB2Extracter::FB2Extracter(QFile* file) {
  QXmlStreamReader xml(file);
  QStringList thisToken;
  QString imgId, imgType;
  while (!xml.atEnd()) {
    switch (xml.readNext()) {
      case QXmlStreamReader::NoToken:
        break;
      case QXmlStreamReader::StartDocument:
        book_ = "<!DOCTYPE HTML><html><body style=\"font-size:14px\">";
        break;
      case QXmlStreamReader::EndDocument:
        book_.append("</body></html>");
        break;
      case QXmlStreamReader::StartElement:
        thisToken.append(xml.name().toString());
        if (xml.name().toString() == "") {

        }
        if (xml.name().toString() == "image") { // расположение рисунков
          if (xml.attributes().count() > 0)
            book_.append("<p align=\"center\">" + xml.attributes().at(0).value().toString() + "</p>");
        }
        if (xml.name() == "binary") { // хранилище рисунков
          imgId = xml.attributes().at(0).value().toString();
          imgType = xml.attributes().at(1).value().toString();
        }
        break;
      case QXmlStreamReader::EndElement:
        if (thisToken.last() == xml.name().toString())
          thisToken.removeLast();
        break;
      case QXmlStreamReader::Characters:
        if (xml.text().toString().contains(QRegExp("[A-Z]|[a-z]|[А-Я]|[а-я]"))) { // если есть текст в блоке
          if (thisToken.contains("description")) { // ОПИСАНИЕ КНИГИ
            break; // не выводим
          }
          if (thisToken.contains("div"))
            break;
          if (!thisToken.contains("binary"))
            book_.append("<p>" + xml.text().toString() + "</p>");
        }
        if (thisToken.contains("binary")) { //для рисунков
          qDebug() << "read image";
          QString image = "<img src=\"data:"
                          + imgType + ";base64,"
                          + xml.text().toString()
                          + "\"/>";
          book_.replace("#" + imgId, image);
        }
        break;
      default:
        break;
    }

  }
}
