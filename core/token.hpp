#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <QString>
#include <memory>
#include <sstream>
#include <utility>

static std::array operators{ '*', '/', '+', '-' };
static std::array separators{ '(', ')' };

enum Associativity
{
  LEFT,
  RIGHT,
};

static bool
is_number(const QString& str)
{
  double dub;
  std::string std_str = str.toStdString();
  if (std_str.rfind('.', 0) == 0) {
    std_str.insert(0, 1, '0');
  }
  std::istringstream iss(std_str);
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

class Token
{
public:
  explicit Token(QString val)
    : value{ std::move(val) }
  {
  }

  virtual ~Token() = default;

  QString value;
};

class Operator final : public Token
{
public:
  explicit Operator(const QString& val);

  uint8_t precedence{};
  Associativity associativity;
};

typedef std::shared_ptr<Token> TokenPtr;
typedef std::shared_ptr<Operator> OperatorPtr;

#endif /* TOKEN_HPP */
