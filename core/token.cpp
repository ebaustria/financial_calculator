#include "token.hpp"

Operator::Operator(const QString& val)
  : Token(val)
{
  if (val == "+" || val == "-") {
    precedence = 2;
  } else {
    precedence = 3;
  }
  associativity = LEFT;
}
