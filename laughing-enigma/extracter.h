#pragma once

#include <QString>
#include <vector>

class Extracter {
 private:
  std::vector<QString> desc_, body_;
 public:
  Extracter();
  const std::vector<QString>& GetDesc() const;
  const std::vector<QString>& GetBody() const;
};

