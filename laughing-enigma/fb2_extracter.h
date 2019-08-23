#pragma once

#include "extracter.h"

#include <QXmlStreamReader>

#include <istream>

class FB2Extracter : public Extracter {
 public:
  FB2Extracter(QXmlStreamReader& read);
};
