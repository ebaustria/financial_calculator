#include "core/calc_helper_functions.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cmath>

#include "core/algo.hpp"

#include "test_helpers.hpp"

TEST_CASE("Check tokenization for single binary integer operation")
{
  const QString str{ "2*6" };
  const std::vector<TokenPtr> tokens = tokenize(str);
  REQUIRE(tokens.size() == 3);
  CHECK(tokens[0]->value == "2");
  CHECK(tokens[1]->value == "*");
  CHECK(tokens[2]->value == "6");
}

TEST_CASE("Check tokenization for single floating point operation")
{
  const QString str{ "0.123+52.088" };
  const std::vector<TokenPtr> tokens = tokenize(str);
  REQUIRE(tokens.size() == 3);
  CHECK(tokens[0]->value == "0.123");
  CHECK(tokens[1]->value == "+");
  CHECK(tokens[2]->value == "52.088");
}

TEST_CASE("Check tokenization for operation that begins with parentheses")
{
  const QString str{ "(4/2)*23" };
  const std::vector<TokenPtr> tokens = tokenize(str);
  REQUIRE(tokens.size() == 7);
  CHECK(tokens[0]->value == "(");
  CHECK(tokens[1]->value == "4");
  CHECK(tokens[2]->value == "/");
  CHECK(tokens[3]->value == "2");
  CHECK(tokens[4]->value == ")");
  CHECK(tokens[5]->value == "*");
  CHECK(tokens[6]->value == "23");
}

TEST_CASE("Check tokenization for operation that ends with parentheses")
{
  const QString str{ "2*(13+24)" };
  const std::vector<TokenPtr> tokens = tokenize(str);
  REQUIRE(tokens.size() == 7);
  CHECK(tokens[0]->value == "2");
  CHECK(tokens[1]->value == "*");
  CHECK(tokens[2]->value == "(");
  CHECK(tokens[3]->value == "13");
  CHECK(tokens[4]->value == "+");
  CHECK(tokens[5]->value == "24");
  CHECK(tokens[6]->value == ")");
}

TEST_CASE("Check reverse Polish result for a single binary integer operation")
{
  const TokenPtr operand_a{ new Token{ QString{ "6" } } };
  const TokenPtr operand_b{ new Token{ "155" } };
  const TokenPtr op{ new Token{ "+" } };

  std::queue<TokenPtr> reverse_polish_equation;
  reverse_polish_equation.push(operand_a);
  reverse_polish_equation.push(operand_b);
  reverse_polish_equation.push(op);

  const QString result = reverse_polish(reverse_polish_equation);
  CHECK(result.toStdString() == "161");
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

  const QString result = reverse_polish(reverse_polish_equation);
  CHECK(result.toStdString() == "59");
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

  const QString result = reverse_polish(reverse_polish_equation);
  CHECK(result.toStdString() == "1018.03282");
}

TEST_CASE("Check intermediate result for floating point addition")
{
  const TokenPtr operand_a{ new Token{ "3.33" } };
  const TokenPtr operand_b{ new Token{ "4.5" } };
  const TokenPtr op{ new Token{ "+" } };
  const double result = intermediate_result(operand_a, operand_b, op);
  constexpr double expected_result = 7.83;
  CHECK(effectively_equal(result, expected_result) == true);
}

TEST_CASE("Check intermediate result for floating point subtraction")
{
  const TokenPtr operand_a{ new Token{ "2.36" } };
  const TokenPtr operand_b{ new Token{ "10.87" } };
  const TokenPtr op{ new Token{ "-" } };
  const double result = intermediate_result(operand_a, operand_b, op);
  constexpr double expected_result = -8.51;
  CHECK(effectively_equal(result, expected_result) == true);
}

TEST_CASE("Check intermediate result for floating point multiplication")
{
  const TokenPtr operand_a{ new Token{ "0.79" } };
  const TokenPtr operand_b{ new Token{ "1.6" } };
  const TokenPtr op{ new Token{ "*" } };
  const double result = intermediate_result(operand_a, operand_b, op);
  constexpr double expected_result = 1.264;
  CHECK(effectively_equal(result, expected_result) == true);
}

TEST_CASE("Check intermediate result for floating point division")
{
  const TokenPtr operand_a{ new Token{ "7.3" } };
  const TokenPtr operand_b{ new Token{ "2.96" } };
  const TokenPtr op{ new Token{ "/" } };
  const double result = intermediate_result(operand_a, operand_b, op);
  constexpr double expected_result = 2.4662162162;
  CHECK(effectively_equal(result, expected_result, 1e-10f) == true);
}

TEST_CASE("Check intermediate result for integer addition")
{
  const TokenPtr operand_a{ new Token{ "3" } };
  const TokenPtr operand_b{ new Token{ "4" } };
  const TokenPtr op{ new Token{ "+" } };
  const double result = intermediate_result(operand_a, operand_b, op);
  CHECK(result == 7);
}

TEST_CASE("Check intermediate result for integer subtraction")
{
  const TokenPtr operand_a{ new Token{ "2" } };
  const TokenPtr operand_b{ new Token{ "10" } };
  const TokenPtr op{ new Token{ "-" } };
  const double result = intermediate_result(operand_a, operand_b, op);
  CHECK(result == -8);
}

TEST_CASE("Check intermediate result for integer multiplication")
{
  const TokenPtr operand_a{ new Token{ "12" } };
  const TokenPtr operand_b{ new Token{ "-43" } };
  const TokenPtr op{ new Token{ "*" } };
  const double result = intermediate_result(operand_a, operand_b, op);
  CHECK(result == -516);
}

TEST_CASE("Check intermediate result for integer division")
{
  const TokenPtr operand_a{ new Token{ "49" } };
  const TokenPtr operand_b{ new Token{ "7" } };
  const TokenPtr op{ new Token{ "/" } };
  const double result = intermediate_result(operand_a, operand_b, op);
  CHECK(result == 7);
}

TEST_CASE("Check shunting yard result")
{
  const QString str{ "2*(13+24)" };
  const std::vector<TokenPtr> tokens = tokenize(str);
  std::queue<TokenPtr> token_queue = shunting_yard(tokens);
  REQUIRE(token_queue.size() == 5);
  for (constexpr std::array expected_val_order{ "2", "13", "24", "+", "*" };
       auto val : expected_val_order) {
    CHECK(token_queue.front()->value == val);
    token_queue.pop();
  }
}

// TEST_CASE("Check equation that starts with '('")
// {
//   const TokenPtr op_a{ new Token{ "(" } };
//   const TokenPtr operand_a{ new Token{ "7" } };
//   const OperatorPtr op_b{ new Operator{ "+" } };
//   const TokenPtr operand_b{ new Token{ "4" } };
//   const TokenPtr op_c{ new Token{ ")" } };
//   const OperatorPtr
// }
