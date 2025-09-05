#include "core/token.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Check if integer token is number")
{
  Token token{ "7" };
  CHECK(token.is_number() == true);
}

TEST_CASE("Check if floating point token is number")
{
  Token token{ "0.2" };
  CHECK(token.is_number() == true);
}

TEST_CASE("Check if floating point token without leading 0 is number")
{
  Token token{ ".57" };
  CHECK(token.is_number() == true);
}

TEST_CASE("Check if non-number token is number")
{
  Token token{ "(" };
  CHECK(token.is_number() == false);
}

TEST_CASE("Check if operator is number")
{
  Operator op{ "+" };
  CHECK(op.is_number() == false);
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
