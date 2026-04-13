#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <QString>
#include <memory>
#include <utility>

enum Associativity
{
  LEFT,
  RIGHT,
};

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
