#include "token.hpp"
#include <sstream>

bool
Token::is_number()
{
  double dub;
  if (value.starts_with('.')) {
    value.insert(0, 1, '0');
  }
  std::istringstream iss(value);
  iss >> std::noskipws >> dub;
  return iss.eof() && !iss.fail();
}

Operator::Operator(const std::string& val)
  : Token(val)
{
  if (val == "+" || val == "-") {
    precedence = 2;
  } else {
    precedence = 3;
  }
  associativity = LEFT;
}

bool
Operator::is_number()
{
  return false;
}