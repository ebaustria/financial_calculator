#include "core/token.hpp"
#include <catch2/catch_test_macros.hpp>

#include "core/calc_helper_functions.hpp"

TEST_CASE("Check if integer token is number")
{
  Token token{ "7" };
  CHECK(is_number(std::make_shared<Token>(token)) == true);
}

TEST_CASE("Check if negative integer token is number")
{
  Token token{ "-101" };
  CHECK(is_number(std::make_shared<Token>(token)) == true);
}

TEST_CASE("Check if floating point token is number")
{
  Token token{ "0.2" };
  CHECK(is_number(std::make_shared<Token>(token)) == true);
}

TEST_CASE("Check if negative floating point token is number")
{
  Token token{ "-37.89" };
  CHECK(is_number(std::make_shared<Token>(token)) == true);
}

TEST_CASE("Check if floating point token without leading 0 is number")
{
  Token token{ ".57" };
  CHECK(is_number(std::make_shared<Token>(token)) == true);
}

TEST_CASE("Check if non-number token is number")
{
  Token token{ "(" };
  CHECK(is_number(std::make_shared<Token>(token)) == false);
}

TEST_CASE("Check if operator is number")
{
  Operator op{ "+" };
  CHECK(is_number(std::make_shared<Operator>(op)) == false);
}

TEST_CASE("Check if '+' operator has correct member variables")
{
  const Operator op{ "+" };
  CHECK(op.associativity == LEFT);
  CHECK(op.precedence == 2);
}

TEST_CASE("Check if '-' operator has correct member variables")
{
  const Operator op{ "-" };
  CHECK(op.associativity == LEFT);
  CHECK(op.precedence == 2);
}

TEST_CASE("Check if '*' operator has correct member variables")
{
  const Operator op{ "*" };
  CHECK(op.associativity == LEFT);
  CHECK(op.precedence == 3);
}

TEST_CASE("Check if '/' operator has correct member variables")
{
  const Operator op{ "/" };
  CHECK(op.associativity == LEFT);
  CHECK(op.precedence == 3);
}
