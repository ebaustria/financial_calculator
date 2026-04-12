#ifndef CALC_HELPER_FUNCTIONS_HPP
#define CALC_HELPER_FUNCTIONS_HPP

#include <QLineEdit>

#include "core/token.hpp"

static Operator multiplication{ "*" };
static Operator division{ "/" };
static Operator addition{ "+" };
static Operator subtraction{ "-" };
static Token left_par{ "(" };
static Token right_par{ ")" };

static std::array<Token*, 6> operators{
  &multiplication, &division, &addition, &subtraction, &left_par, &right_par
};

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
is_operator(const QString& str)
{
  bool is_operator{ false };
  for (const Token* op : operators) {
    if (str == op->value) {
      is_operator = true;
      break;
    }
  }
  return is_operator;
}


#endif /* CALC_HELPER_FUNCTIONS_HPP */
