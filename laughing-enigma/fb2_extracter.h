#pragma once

#include "extracter.h"

#include <QFile>
#include <QXmlStreamReader>

#include <string>

namespace FB2Parser {
const int font_size = 14;
class Base {
 public:
  Base(QXmlStreamReader&);

  virtual void Start(QString&) = 0;
  virtual void End(QString&) = 0;
  virtual ~Base() = default;

 protected:
  QXmlStreamReader& xml;
};

class Document : public Base {
 public:
  Document(QXmlStreamReader& xml);

  void Start(QString&) override;
  void End(QString&) override;

  ~Document() override = default;
};
} // namespace FB2Parser

class FB2Extracter : public Extracter {
 public:
  FB2Extracter(QFile* file);
};
