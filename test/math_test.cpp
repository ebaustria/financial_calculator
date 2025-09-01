#include "core/calc_helper_functions.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cmath>

bool
effectively_equal(const double a, const double b, const double epsilon = 1e-12f)
{
  const double res = std::fabs(a - b);
  return res < epsilon;
}

TEST_CASE("Check reverse Polish result for a single binary integer operation")
{
  const TokenPtr operand_a{ new Token{ "6" } };
  const TokenPtr operand_b{ new Token{ "155" } };
  const TokenPtr op{ new Token{ "+" } };

  std::queue<TokenPtr> reverse_polish_equation;
  reverse_polish_equation.push(operand_a);
  reverse_polish_equation.push(operand_b);
  reverse_polish_equation.push(op);

  std::string result = reverse_polish(reverse_polish_equation);
  CHECK(result == "161.000000");
}

TEST_CASE("Check reverse Polish result for two binary integer operations")
{
  const TokenPtr operand_a{ new Token{ "156" } };
  const TokenPtr operand_b{ new Token{ "3" } };
  const TokenPtr op{ new Token{ "/" } };
  const TokenPtr operand_c{ new Token{ "7" } };
  const TokenPtr op_2{ new Token{ "+" } };

  std::queue<TokenPtr> reverse_polish_equation;
  reverse_polish_equation.push(operand_a);
  reverse_polish_equation.push(operand_b);
  reverse_polish_equation.push(op);
  reverse_polish_equation.push(operand_c);
  reverse_polish_equation.push(op_2);

  std::string result = reverse_polish(reverse_polish_equation);
  CHECK(result == "59.000000");
}

TEST_CASE(
  "Check reverse Polish result for a single binary floating point operation")
{
  const TokenPtr operand_a{ new Token{ "6.554" } };
  const TokenPtr operand_b{ new Token{ "155.33" } };
  const TokenPtr op{ new Token{ "*" } };

  std::queue<TokenPtr> reverse_polish_equation;
  reverse_polish_equation.push(operand_a);
  reverse_polish_equation.push(operand_b);
  reverse_polish_equation.push(op);

  std::string result = reverse_polish(reverse_polish_equation);
  CHECK(result == "1018.032820");
}

TEST_CASE("Check floating point addition")
{
  const TokenPtr operand_a{ new Token{ "3.33" } };
  const TokenPtr operand_b{ new Token{ "4.5" } };
  const TokenPtr op{ new Token{ "+" } };
  const double result = intermediate_result(operand_a, operand_b, op);
  constexpr double expected_result = 7.83;
  CHECK(effectively_equal(result, expected_result) == true);
}

TEST_CASE("Check floating point subtraction")
{
  const TokenPtr operand_a{ new Token{ "2.36" } };
  const TokenPtr operand_b{ new Token{ "10.87" } };
  const TokenPtr op{ new Token{ "-" } };
  const double result = intermediate_result(operand_a, operand_b, op);
  constexpr double expected_result = -8.51;
  CHECK(effectively_equal(result, expected_result) == true);
}

TEST_CASE("Check floating point multiplication")
{
  const TokenPtr operand_a{ new Token{ "0.79" } };
  const TokenPtr operand_b{ new Token{ "1.6" } };
  const TokenPtr op{ new Token{ "*" } };
  const double result = intermediate_result(operand_a, operand_b, op);
  constexpr double expected_result = 1.264;
  CHECK(effectively_equal(result, expected_result) == true);
}

TEST_CASE("Check floating point division")
{
  const TokenPtr operand_a{ new Token{ "7.3" } };
  const TokenPtr operand_b{ new Token{ "2.96" } };
  const TokenPtr op{ new Token{ "/" } };
  const double result = intermediate_result(operand_a, operand_b, op);
  constexpr double expected_result = 2.4662162162;
  CHECK(effectively_equal(result, expected_result, 1e-10f) == true);
}

TEST_CASE("Check integer addition")
{
  const TokenPtr operand_a{ new Token{ "3" } };
  const TokenPtr operand_b{ new Token{ "4" } };
  const TokenPtr op{ new Token{ "+" } };
  const double result = intermediate_result(operand_a, operand_b, op);
  CHECK(result == 7);
}

TEST_CASE("Check integer subtraction")
{
  const TokenPtr operand_a{ new Token{ "2" } };
  const TokenPtr operand_b{ new Token{ "10" } };
  const TokenPtr op{ new Token{ "-" } };
  const double result = intermediate_result(operand_a, operand_b, op);
  CHECK(result == -8);
}

TEST_CASE("Check integer multiplication")
{
  const TokenPtr operand_a{ new Token{ "12" } };
  const TokenPtr operand_b{ new Token{ "-43" } };
  const TokenPtr op{ new Token{ "*" } };
  const double result = intermediate_result(operand_a, operand_b, op);
  CHECK(result == -516);
}

TEST_CASE("Check integer division")
{
  const TokenPtr operand_a{ new Token{ "49" } };
  const TokenPtr operand_b{ new Token{ "7" } };
  const TokenPtr op{ new Token{ "/" } };
  const double result = intermediate_result(operand_a, operand_b, op);
  CHECK(result == 7);
}
