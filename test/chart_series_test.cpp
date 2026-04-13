#include "core/chart_series.hpp"

#include <catch2/catch_test_macros.hpp>

#include "test_helpers.hpp"

void
check_final_strat_principal(Strategy* strategy, const uint32_t total_years)
{
  const QList<QPointF> future_values = strategy->calculate_all();
  for (uint32_t period = 0; period <= total_years; ++period) {
    const double expected = strategy->calculate(period);
    CHECK(future_values.at(period).x() == period);
    CHECK(future_values.at(period).y() == expected);
  }
  CHECK(strategy->min_x == 0);
  CHECK(strategy->max_x == total_years);
  CHECK(strategy->min_y == strategy->principal);
  CHECK(strategy->max_y == future_values.at(total_years).y());
}

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
  check_final_strat_principal(&simple_interest_strategy, total_years);
}

TEST_CASE("Check compounding interest principal after four years")
{
  constexpr uint32_t total_years = 4;
  CompoundingInterestStrategy compound_interest_strategy{
    2'000.0, 3.2, 3.0, total_years
  };
  check_final_strat_principal(&compound_interest_strategy, total_years);
}

TEST_CASE("Loan should not amortize with principal 1000, interest rate 5%, payment 3")
{
  const LoanRepaymentStrategy loan_repayment_strategy{ 1'000.0, 5.0, 3.0 };
  CHECK(loan_repayment_strategy.is_amortizing() == false);
}

TEST_CASE("Loan should amortize with principal 1000, interest rate 5%, payment 5")
{
  const LoanRepaymentStrategy loan_repayment_strategy{ 1'000.0, 5.0, 5.0 };
  CHECK(loan_repayment_strategy.is_amortizing() == true);
}