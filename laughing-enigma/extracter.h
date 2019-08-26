#pragma once

#include <QString>
#include <vector>

class Extracter {
 protected:
  QString book_;
  std::vector<QString> images_;
 public:
  Extracter();
  const QString& GetBook() const;
  const std::vector<QString>& GetImages() const;
};

