#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <memory>

enum Associativity
{
    LEFT,
    RIGHT,
};

class Token
{
public:
    explicit Token(std::string val) : value{std::move(val)}
    {
    }

    virtual ~Token() = default;
    virtual bool is_number();

    std::string value;
};

class Operator final : public Token
{
public:
    explicit Operator(const std::string& val);
    bool is_number() override;

    uint8_t precedence{};
    Associativity associativity;
};

typedef std::shared_ptr<Token> TokenPtr;
typedef std::shared_ptr<Operator> OperatorPtr;

#endif /* TOKEN_HPP */
