#include "extracter.h"

using namespace std;

Extracter::Extracter() {}

const vector<QString>& Extracter::GetDesc() const {
  return desc_;
}

const vector<QString>& Extracter::GetBody() const {
  return body_;
}
