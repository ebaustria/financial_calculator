#ifndef CALC_HELPER_FUNCTIONS_HPP
#define CALC_HELPER_FUNCTIONS_HPP

#include "core/token.hpp"

static double intermediate_result(const TokenPtr& operand_a, const TokenPtr& operand_b, const TokenPtr& op)
{
    const double a = std::stod(operand_a->value);
    const double b = std::stod(operand_b->value);
    if (op->value == "+")
    {
        return a + b;
    }
    if (op->value == "-")
    {
        return a - b;
    }
    if (op->value == "*")
    {
        return a * b;
    }
    return a / b;
}

#endif /* CALC_HELPER_FUNCTIONS_HPP */