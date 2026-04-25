#include "core/strategy.hpp"

#include <catch2/catch_test_macros.hpp>

#include "test_helpers.hpp"

void
check_final_strat_principal(FinancialInstrumentStrat* strategy, const uint32_t total_years)
{
  const QList<QPointF> future_values = strategy->calculate_all();
  REQUIRE(future_values.size() == total_years + 1);

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
  SimpleInterestStrategy simple_interest_strategy{ QString{ "1000.0" },
                                                   QString{ "5.0" },
                                                   QString{ "20" } };
  const double future_value = simple_interest_strategy.calculate(10);
  CHECK(future_value == 1'500.0);
}

TEST_CASE("Check compounding interest calculation")
{
  CompoundingInterestStrategy compounding_interest_strategy{
    QString{ "2000.0" }, QString{ "2.5" }, QString{ "4.0" }, QString{ "10" }
  };
  const double future_value = compounding_interest_strategy.calculate(5);
  CHECK(effectively_equal(future_value, 2'265.4154767858377) == true);
}

TEST_CASE("Check amortizing loan repayment calculation")
{
  LoanRepaymentStrategy loan_repayment_strategy{ QString{ "100000.0" },
                                                 QString{ "4.0" },
                                                 QString{ "800.0" } };
  const double balance = loan_repayment_strategy.calculate(24);
  CHECK(effectively_equal(balance, 88'359.985717730728) == true);
}

TEST_CASE("Check non-amortizing loan repayment calculation")
{
  LoanRepaymentStrategy loan_repayment_strategy{ QString{ "100000.0" },
                                                 QString{ "4.0" },
                                                 QString{ "300.0" } };
  const double balance = loan_repayment_strategy.calculate(24);
  CHECK(effectively_equal(balance, 100'831.42959159067) == true);
}

TEST_CASE("Check simple interest principal after three years")
{
  SimpleInterestStrategy simple_interest_strategy{ QString{ "1000.0" },
                                                   QString{ "3.7" },
                                                   QString{ "3" } };
  check_final_strat_principal(&simple_interest_strategy, 3);
}

TEST_CASE("Check compounding interest principal after four years")
{
  CompoundingInterestStrategy compound_interest_strategy{
    QString{ "2000.0" }, QString{ "3.2" }, QString{ "3.0" }, QString{ "4" }
  };
  check_final_strat_principal(&compound_interest_strategy, 4);
}

TEST_CASE(
  "Loan should not amortize with principal 1000, interest rate 5%, payment 3")
{
  const LoanRepaymentStrategy loan_repayment_strategy{ QString{ "1000.0" },
                                                       QString{ "5.0" },
                                                       QString{ "3.0" } };
  CHECK(loan_repayment_strategy.is_amortizing() == false);
}

TEST_CASE(
  "Loan should amortize with principal 1000, interest rate 5%, payment 5")
{
  const LoanRepaymentStrategy loan_repayment_strategy{ QString{ "1000.0" },
                                                       QString{ "5.0" },
                                                       QString{ "5.0" } };
  CHECK(loan_repayment_strategy.is_amortizing() == true);
}

TEST_CASE("Check full repayment of amortizing loan")
{
  LoanRepaymentStrategy loan_repayment_strategy{ QString{ "500.0" },
                                                 QString{ "6.0" },
                                                 QString{ "100.0" } };
  const QList<QPointF> future_values = loan_repayment_strategy.calculate_all();
  REQUIRE(future_values.size() == 7);
  for (const QPointF& point : future_values) {
    const double expected_value =
      loan_repayment_strategy.calculate(static_cast<uint32_t>(point.x()));
    CHECK(point.y() == expected_value);
  }

  CHECK(loan_repayment_strategy.min_x == 0);
  CHECK(loan_repayment_strategy.max_x == 6);
  CHECK(loan_repayment_strategy.min_y == 0.0);
  CHECK(loan_repayment_strategy.max_y == future_values.at(0).y());
}

TEST_CASE("Check status of non-amortizing loan after max periods")
{
  LoanRepaymentStrategy loan_repayment_strategy{ QString{ "500.0" },
                                                 QString{ "6.0" },
                                                 QString{ "2.0" } };
  const QList<QPointF> future_values = loan_repayment_strategy.calculate_all();
  REQUIRE(future_values.size() == 121);
  for (const QPointF& point : future_values) {
    const double expected_value =
      loan_repayment_strategy.calculate(static_cast<uint32_t>(point.x()));
    CHECK(point.y() == expected_value);
  }

  CHECK(loan_repayment_strategy.min_x == 0);
  CHECK(loan_repayment_strategy.max_x == 120);
  CHECK(loan_repayment_strategy.min_y == 500.0);
  CHECK(loan_repayment_strategy.max_y == future_values.at(120).y());
}
