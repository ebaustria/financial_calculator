#include "token.hpp"

bool Token::is_number()
{
    char* p;
    strtof(value.c_str(), &p);
    return *p == 0;
}

Operator::Operator(const std::string& val) : Token(val)
{
    if (val == "+" || val == "-")
    {
        precedence = 2;
    }
    else
    {
        precedence = 3;
    }
    associativity = LEFT;
}

bool Operator::is_number()
{
    return false;
}