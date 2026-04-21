#ifndef FINANCIAL_CALCULATOR_TEST_FIXTURE_HPP
#define FINANCIAL_CALCULATOR_TEST_FIXTURE_HPP
#include "app/calculator.hpp"
#include "ui_calculator_window.h"

class TestFixture
{
public:
  TestFixture();
  static void check_internet();
  static void check_conversion_recalculation(float first_conversion,
                                             float second_conversion);
  [[nodiscard]] float get_current_conversion_result() const;
  Calculator* calculator{ nullptr };

private:
  QMainWindow window;
  Ui::FinancialCalculatorWindow fin_calc_window{};
};

#endif // FINANCIAL_CALCULATOR_TEST_FIXTURE_HPP
