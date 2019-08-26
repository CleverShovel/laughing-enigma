#include "extracter.h"

using namespace std;

Extracter::Extracter() {}

const QString& Extracter::GetBook() const {
  return book_;
}

const vector<QString>& Extracter::GetImages() const {
  return images_;
}
