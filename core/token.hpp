#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <QString>
#include <memory>

enum Associativity
{
  LEFT,
  RIGHT,
};

class Token
{
public:
  explicit Token(const QString& val)
    : value{ val }
  {
  }

  virtual ~Token() = default;
  virtual bool is_number();

  QString value;
};

class Operator final : public Token
{
public:
  explicit Operator(const QString& val);
  bool is_number() override;

  uint8_t precedence{};
  Associativity associativity;
};

typedef std::shared_ptr<Token> TokenPtr;
typedef std::shared_ptr<Operator> OperatorPtr;

#endif /* TOKEN_HPP */
