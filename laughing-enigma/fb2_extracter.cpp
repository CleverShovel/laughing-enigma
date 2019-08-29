#include "fb2_extracter.h"

#include <QDebug>

#include <unordered_set>

using namespace std;

namespace FB2Parse {
const unordered_map <QString, decltype(&Create<Base>)> tags = {
  {"FictionBook",   &Create <FictionBook>},
  {"description",   &Create<Description>},
  {"title-info",    &Create<TitleInfo>},
  {"coverpage",     &Create<CoverPage>},
  {"body",          &Create<Body>},
  {"title",         &Create<Title>},
  {"section",       &Create<Section>},
  {"image",         &Create<Image>},
  {"binary",        &Create<Binary>},
  {"p",             &Create<P>},
  {"a",             &Create<A>},
  {"table",         &Create<Table>},
  {"tr",            &Create<Tr>},
  {"td",            &Create<Td>},
  {"th",            &Create<Th>},
  {"sub",           &Create<Sub>},
  {"sup",           &Create<Sup>},
  {"code",          &Create<Code>},
  {"strong",        &Create<Strong>},
  {"cite",          &Create<Cite>},
  {"empty-line",    &Create<EmptyLine>},
  {"emphasis",      &Create<Emphasis>},
  {"strikethrough", &Create<Strikethrough>},
  {"default",       &Create<Redunant>}
};

Reader::Reader(QXmlStreamReader* xml) : xml(xml) {}

QString arg_variable_size(QString fmt, std::vector<QString> args) {
  return std::accumulate(
             std::make_move_iterator(args.begin()),
             std::make_move_iterator(args.end()),
             std::move(fmt),
  [](QString toFmt, QString arg) {
    return toFmt.arg(arg);
  }
         );
}

QString Reader::Parse() {
  ReadTokens();

  return arg_variable_size(move(str), move(args));
}

void Reader::ReadTokens() {
  while (!xml->atEnd()) {
    auto token = xml->readNext();
    if (token == QXmlStreamReader::StartDocument) {
      elems.emplace_back(new Document(*this));
      elems.back()->Start(str);
    }
    if (token == QXmlStreamReader::EndDocument) {
      elems.back()->End(str);
      elems.pop_back();
    }
    if (token == QXmlStreamReader::StartElement) {
      auto tag = xml->name().toString();
      qDebug() << '+' + tag;
      if (elems.back()->GetTag() == "")
        elems.push_back(tags.at("default")(*this));
      else if (tags.count(tag))
        elems.push_back(tags.at(tag)(*this));
      else
        elems.push_back(tags.at("default")(*this));
      elems.back()->Start(str);
    }
    if (token == QXmlStreamReader::EndElement) {
      qDebug() << '-' + xml->name().toString();
      elems.back()->End(str);
      elems.pop_back();
    }
    if (token == QXmlStreamReader::Characters) {
//      if (xml->text().toString().contains(QRegExp("[A-Z]|[a-z]|[А-Я]|[а-я]"))) {
//        str += "<p>" + xml->text().toString() + "</p>";
//      }
      elems.back()->Process(str);
    }
  }
}

QXmlStreamReader& Reader::GetXml() { return *xml; }

void Reader::AddArg(QString arg) {
  args.push_back(move(arg));
}

std::unordered_map<QString, int>& Reader::GetImages() {
  return images;
}

Base::Base(Reader& reader, QString tag = "") : reader(reader), tag(move(tag)) {}

void Base::Start(QString&) {}
void Base::Process(QString&) {}
void Base::End(QString&) {}
const QString& Base::GetTag() const { return tag; }

Document::Document(Reader& reader) : Base(reader, "Document") {}

void Document::Start(QString& str) {
  str.reserve(str.size() + 60);
  str += "<!DOCTYPE HTML><html><body style=\"font-size:";
  str += QString::number(text_font_size);
  str += "px\">";
}

void Document::End(QString& str) {
  str += "</body></html>";
}

FictionBook::FictionBook(Reader& reader) : Base(reader, "FictionBook") {}

TitleInfo::TitleInfo(Reader& reader) : Base(reader, "title-info") {}

CoverPage::CoverPage(Reader& reader) : Base(reader, "coverpage") {}

Description::Description(Reader& reader) : Tag(reader, "description") {}
//TitleInfo::TitleInfo(Reader& reader) : Base(reader) {}

Tag::Tag(Reader& reader, QString tag) : Base(reader, move(tag)) {}

void Tag::Start(QString& str) {
  str.reserve(str.size() + 2 + tag.size());
  str += "<";
  str += tag;
  str += ">";
}

void Tag::Process(QString& str) {
  str += reader.GetXml().text().toString();
}

void Tag::End(QString& str) {
  str.reserve(str.size() + 3 + tag.size());
  str += "</";
  str += tag;
  str += ">";
}

Body::Body(Reader& reader) : Base(reader, "body") {}

P::P(Reader& reader)                            : Tag(reader, "p") {}
A::A(Reader& reader)                            : Tag(reader, "a") {}
Table::Table(Reader& reader)                    : Tag(reader, "table") {}
Tr::Tr(Reader& reader)                          : Tag(reader, "tr") {}
Td::Td(Reader& reader)                          : Tag(reader, "td") {}
Th::Th(Reader& reader)                          : Tag(reader, "th") {}
Sub::Sub(Reader& reader)                        : Tag(reader, "sub") {}
Sup::Sup(Reader& reader)                        : Tag(reader, "sup") {}
Code::Code(Reader& reader)                      : Tag(reader, "code") {}
Strong::Strong(Reader& reader)                  : Tag(reader, "strong") {}
Cite::Cite(Reader& reader)                      : Tag(reader, "cite") {}

EmptyLine::EmptyLine(Reader& reader)            : Tag(reader, "br") {}
Emphasis::Emphasis(Reader& reader)              : Tag(reader, "i") {}
Strikethrough::Strikethrough(Reader& reader)    : Tag(reader, "strike") {}

Image::Image(Reader& reader) : Tag(reader, "image") {}

void Image::Start(QString& str) {
  if (reader.GetXml().attributes().count() > 0) {
    auto attr = reader.GetXml().attributes().at(0).value().toString();
    attr.remove(0, 1);
    qDebug() << attr;
    auto& images = reader.GetImages();
    if (images.count(attr) == 0)
      images[attr] = images.size();
    str.reserve(str.size() + 23 + attr.size());
    str += "<p align=\"center\">%";
    str += QString::number(images[attr]);
    str += "</p>";
  }
}

void Image::Process(QString&) {}
void Image::End(QString&) {}

Title::Title(Reader& reader) : Tag(reader, "title") {}

void Title::Start(QString& str) {
  str.reserve(str.size() + 50);
  qDebug() << QString("<p align=\"center\" style=\"font-size:") +
           QString::number(title_font_size) + "px\">";
  str += "<p align=\"center\" style=\"font-size:";
  str += QString::number(title_font_size);
  str += "px\">";
}

void Title::End(QString& str) {
  str += "</p>";
}

Section::Section(Reader& reader) : Base(reader, "section") {}

Binary::Binary(Reader& reader) : Tag(reader, "binary") {}

void Binary::Start(QString&) {
//  TODO: change this
  auto& xml = reader.GetXml();
  id = xml.attributes().at(0).value().toString();
  type = xml.attributes().at(1).value().toString();
//  xml.readNext();
//  QString value;
//  value.reserve(250);
//  value = "<img src=\"data:" +
//          std::move(type) +
//          ";base64," +
//          xml.text().toString()
//          + "\"/>";
//  reader.AddArg(move(value));
}

void Binary::Process(QString&) {
  QString value;
  value.reserve(250);
  value = "<img src=\"data:" +
          std::move(type) +
          ";base64," +
          reader.GetXml().text().toString()
          + "\"/>";
  reader.AddArg(move(value));
}

void Binary::End(QString&) {}

Redunant::Redunant(Reader& reader) : Base(reader) {}
} // FB2Parse


FB2Extracter::FB2Extracter(QFile* file) {
  QXmlStreamReader xml(file);
  FB2Parse::Reader reader(&xml);
  book_ = reader.Parse();
//  QStringList thisToken;
//  QString imgId, imgType;
//  while (!xml.atEnd()) {
//    switch (xml.readNext()) {
//      case QXmlStreamReader::NoToken:
//        break;
//      case QXmlStreamReader::StartDocument:
//        book_ = "<!DOCTYPE HTML><html><body style=\"font-size:14px\">";
//        break;
//      case QXmlStreamReader::EndDocument:
//        book_.append("</body></html>");
//        break;
//      case QXmlStreamReader::StartElement:
//        thisToken.append(xml.name().toString());
//        if (xml.name().toString() == "") {

//        }
//        if (xml.name().toString() == "image") { // расположение рисунков
//          if (xml.attributes().count() > 0)
//            book_.append("<p align=\"center\">" + xml.attributes().at(0).value().toString() + "</p>");
//        }
//        if (xml.name() == "binary") { // хранилище рисунков
//          imgId = xml.attributes().at(0).value().toString();
//          imgType = xml.attributes().at(1).value().toString();
//        }
//        break;
//      case QXmlStreamReader::EndElement:
//        if (thisToken.last() == xml.name().toString())
//          thisToken.removeLast();
//        break;
//      case QXmlStreamReader::Characters:
//        if (xml.text().toString().contains(QRegExp("[A-Z]|[a-z]|[А-Я]|[а-я]"))) { // если есть текст в блоке
//          if (thisToken.contains("description")) { // ОПИСАНИЕ КНИГИ
//            break; // не выводим
//          }
//          if (thisToken.contains("div"))
//            break;
//          if (!thisToken.contains("binary"))
//            book_.append("<p>" + xml.text().toString() + "</p>");
//        }
//        if (thisToken.contains("binary")) { //для рисунков
//          QString image = "<img src=\"data:"
//                          + imgType + ";base64,"
//                          + xml.text().toString()
//                          + "\"/>";
//          book_.replace("#" + imgId, image);
//        }
//        break;
//      default:
//        break;
//    }

//  }
}
