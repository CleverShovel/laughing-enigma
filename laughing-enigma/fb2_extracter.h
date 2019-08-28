#pragma once

#include "extracter.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QString>
#include <QStringView>

#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include <numeric>
#include <utility>
#include <iterator>

namespace std {
template<> struct hash<QString> {
  std::size_t operator()(const QString& s) const {
    return qHash(s);
  }
};
}

namespace FB2Parse {
class Base;
const int title_font_size = 20;
const int text_font_size = 14;

class Reader {
 public:
  Reader() = default;
  Reader(QXmlStreamReader*);
  QString Parse();
  void ReadTokens();
  QXmlStreamReader& GetXml();
  void AddArg(QString);
  std::unordered_map<QString, int>& GetImages();
 private:
  QString str;
  QXmlStreamReader* xml;
  std::vector<std::unique_ptr<Base>> elems;
  std::unordered_map<QString, int> images;
  std::vector<QString> args;
};

template <typename T>
std::unique_ptr<Base> Create(Reader& reader) {
  return std::make_unique<T>(reader);
}

class Base {
 public:
  Base(Reader&, QString);
  virtual void Start(QString&);
  virtual void End(QString&);
  virtual ~Base() = default;
  virtual const QString& GetTag() const;

 protected:
  Reader& reader;
  QString tag;
};

class Document : public Base {
 public:
  Document(Reader&);
  void Start(QString&) override;
  void End(QString&) override;
};

class Tag : public Base {
 public:
  Tag(Reader&, QString);
  void Start(QString&) override;
  void End(QString&) override;
};

class Body : public Base {
 public:
  Body(Reader&);
};

class Description : public Tag {
 public:
  Description(Reader&);
};

class P : public Tag {
 public:
  P(Reader&);
};

class A : public Tag {
 public:
  A(Reader&);
};

class Table : public Tag {
 public:
  Table(Reader&);
};

class Tr : public Tag {
 public:
  Tr(Reader&);
};

class Td : public Tag {
 public:
  Td(Reader&);
};

class Th : public Tag {
 public:
  Th(Reader&);
};

class Sub : public Tag {
 public:
  Sub(Reader&);
};

class Sup : public Tag {
 public:
  Sup(Reader&);
};

class Code : public Tag {
 public:
  Code(Reader&);
};

class Strong : public Tag {
 public:
  Strong(Reader&);
};

class Cite : public Tag {
 public:
  Cite(Reader&);
};

class EmptyLine : public Tag {
 public:
  EmptyLine(Reader&);
};

class Emphasis : public Tag {
 public:
  Emphasis(Reader&);
};

class Strikethrough : public Tag {
 public:
  Strikethrough(Reader&);
};

class Image : public Tag {
 public:
  Image(Reader&);
  void Start(QString&) override;
  void End(QString&) override;
};

class Title : public Tag {
 public:
  Title(Reader&);
  void Start(QString&) override;
  void End(QString&) override;
};

class Binary : public Tag {
 public:
  Binary(Reader&);
  void Start(QString&) override;
  void End(QString&) override;
};

class Redunant : public Base {
 public:
  Redunant(Reader&);
};
} // namespace FB2Parse

class FB2Extracter : public Extracter {
 public:
  FB2Extracter(QFile* file);
};
