#ifndef CALC_HELPER_FUNCTIONS_HPP
#define CALC_HELPER_FUNCTIONS_HPP

#include <QLineEdit>
#include <sstream>

#include "core/token.hpp"

static std::array operators{ '*', '/', '+', '-' };
static std::array separators{ '(', ')' };

static float
text_to_float(const QLineEdit* line_edit)
{
  return line_edit->text().toFloat();
}

static uint8_t
text_to_uint8(const QLineEdit* line_edit)
{
  return line_edit->text().toUInt();
}

static bool
is_number(const TokenPtr& str)
{
  double dub;
  if (str->value.startsWith('.')) {
    str->value.insert(0, '0');
  }
  std::istringstream iss(str->value.toStdString());
  iss >> std::noskipws >> dub;
  return iss.eof() && !iss.fail();
}

static bool
is_operator(const QString& str)
{
  bool is_operator{ false };
  for (const char op : operators) {
    if (str == op) {
      is_operator = true;
      break;
    }
  }
  return is_operator;
}

static bool
is_separator(const QString& str)
{
  bool is_separator{ false };
  for (const char sep : separators) {
    if (str == sep) {
      is_separator = true;
      break;
    }
  }
  return is_separator;
}

#endif /* CALC_HELPER_FUNCTIONS_HPP */
