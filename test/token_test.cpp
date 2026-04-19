#include "core/token.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Check if integer token is number")
{
  const QString str{ "7" };
  CHECK(is_number(str) == true);
}

TEST_CASE("Check if negative integer token is number")
{
  const QString str{ "-101" };
  CHECK(is_number(str) == true);
}

TEST_CASE("Check if floating point token is number")
{
  const QString str{ "0.2" };
  CHECK(is_number(str) == true);
}

TEST_CASE("Check if negative floating point token is number")
{
  const QString str{ "-37.89" };
  CHECK(is_number(str) == true);
}

TEST_CASE("Check if floating point token without leading 0 is number")
{
  const QString str{ ".57" };
  CHECK(is_number(str) == true);
}

TEST_CASE("Check if non-number token is number")
{
  const QString str{ "(" };
  CHECK(is_number(str) == false);
}

TEST_CASE("Check if operator is number")
{
  const QString str{ "+" };
  CHECK(is_number(str) == false);
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
