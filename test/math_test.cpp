#include <cmath>
#include <catch2/catch_test_macros.hpp>
#include "core/calc_helper_functions.hpp"

bool effectively_equal(const double a, const double b, const double epsilon = 1e-12f)
{
    const double res = std::fabs(a - b);
    return res < epsilon;
}

TEST_CASE("Check floating point addition")
{
    const TokenPtr operand_a{new Token{"3.33"}};
    const TokenPtr operand_b{new Token{"4.5"}};
    const TokenPtr op{new Token{"+"}};
    const double result = intermediate_result(operand_a, operand_b, op);
    constexpr double expected_result = 7.83;
    CHECK(effectively_equal(result,expected_result) == true);
}

TEST_CASE("Check floating point subtraction")
{
    const TokenPtr operand_a{new Token{"2.36"}};
    const TokenPtr operand_b{new Token{"10.87"}};
    const TokenPtr op{new Token{"-"}};
    const double result = intermediate_result(operand_a, operand_b, op);
    constexpr double expected_result = -8.51;
    CHECK(effectively_equal(result,expected_result) == true);
}

TEST_CASE("Check floating point multiplication")
{
    const TokenPtr operand_a{new Token{"0.79"}};
    const TokenPtr operand_b{new Token{"1.6"}};
    const TokenPtr op{new Token{"*"}};
    const double result = intermediate_result(operand_a, operand_b, op);
    constexpr double expected_result = 1.264;
    CHECK(effectively_equal(result,expected_result) == true);
}

TEST_CASE("Check floating point division")
{
    const TokenPtr operand_a{new Token{"7.3"}};
    const TokenPtr operand_b{new Token{"2.96"}};
    const TokenPtr op{new Token{"/"}};
    const double result = intermediate_result(operand_a, operand_b, op);
    constexpr double expected_result = 2.4662162162;
    CHECK(effectively_equal(result,expected_result, 1e-10f) == true);
}

TEST_CASE("Check integer addition")
{
    const TokenPtr operand_a{new Token{"3"}};
    const TokenPtr operand_b{new Token{"4"}};
    const TokenPtr op{new Token{"+"}};
    const double result = intermediate_result(operand_a, operand_b, op);
    CHECK(result == 7);
}

TEST_CASE("Check integer subtraction")
{
    const TokenPtr operand_a{new Token{"2"}};
    const TokenPtr operand_b{new Token{"10"}};
    const TokenPtr op{new Token{"-"}};
    const double result = intermediate_result(operand_a, operand_b, op);
    CHECK(result == -8);
}

TEST_CASE("Check integer multiplication")
{
    const TokenPtr operand_a{new Token{"12"}};
    const TokenPtr operand_b{new Token{"-43"}};
    const TokenPtr op{new Token{"*"}};
    const double result = intermediate_result(operand_a, operand_b, op);
    CHECK(result == -516);
}

TEST_CASE("Check integer division")
{
    const TokenPtr operand_a{new Token{"49"}};
    const TokenPtr operand_b{new Token{"7"}};
    const TokenPtr op{new Token{"/"}};
    const double result = intermediate_result(operand_a, operand_b, op);
    CHECK(result == 7);
}
