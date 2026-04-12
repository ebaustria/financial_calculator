#include "core/chart_series.hpp"

#include <catch2/catch_test_macros.hpp>

#include "test_helpers.hpp"

TEST_CASE("Check simple interest calculation")
{
  SimpleInterestStrategy simple_interest_strategy{ 1'000.0, 5.0, 20 };
  const double future_value = simple_interest_strategy.calculate(10);
  CHECK(future_value == 1'500.0);
}

TEST_CASE("Check compounding interest calculation")
{
  CompoundingInterestStrategy compounding_interest_strategy{
    2'000.0, 2.5, 4.0, 10
  };
  const double future_value = compounding_interest_strategy.calculate(5);
  CHECK(effectively_equal(future_value, 2'063.28614809, 1e-9f) == true);
}

TEST_CASE("Check loan repayment calculation")
{
  LoanRepaymentStrategy loan_repayment_strategy{ 100'000.0, 4.0, 800.0 };
  const double balance = loan_repayment_strategy.calculate(24);
  CHECK(effectively_equal(balance, 88'359.985717730728) == true);
}

TEST_CASE("Check simple interest principal after three years")
{
  constexpr uint32_t total_years = 3;
  SimpleInterestStrategy simple_interest_strategy{ 1'000.0, 3.7, total_years };
  const QList<QPointF> future_values = simple_interest_strategy.calculate_all();
  for (uint32_t period = 0; period <= total_years; ++period) {
    const double expected = simple_interest_strategy.calculate(period);
    CHECK(future_values.at(period).x() == period);
    CHECK(future_values.at(period).y() == expected);
  }
  CHECK(simple_interest_strategy.min_x == 0);
  CHECK(simple_interest_strategy.max_x == total_years);
  CHECK(simple_interest_strategy.min_y == 1'000.0);
  CHECK(simple_interest_strategy.max_y == future_values.at(total_years).y());
}