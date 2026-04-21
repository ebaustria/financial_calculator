#ifndef FINANCIAL_CALCULATOR_TEST_FIXTURE_HPP
#define FINANCIAL_CALCULATOR_TEST_FIXTURE_HPP
#include "app/calculator.hpp"
#include "ui_calculator_window.h"

class TestFixture
{
public:
  TestFixture();
  static void check_internet();
  Calculator* calculator{ nullptr };

private:
  QMainWindow window;
  Ui::FinancialCalculatorWindow fin_calc_window{};
};

#endif // FINANCIAL_CALCULATOR_TEST_FIXTURE_HPP
