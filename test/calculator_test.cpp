#include "app/calculator.hpp"
#include "ui_calculator_window.h"
#include <catch2/catch_test_macros.hpp>
#include <cpr/cpr.h>

SCENARIO("The user can convert from one currency to another by changing the "
         "base currency amount")
{
  GIVEN("The calculator app is initialized")
  {
    int argc = 0;
    char* argv[] = {};
    QApplication app{ argc, argv };
    QMainWindow window;
    Ui::FinancialCalculatorWindow fin_calc_window;
    Calculator calculator{ &window };
    fin_calc_window.setupUi(&window);
    window.setCentralWidget(&calculator);
    QString initial_conversion =
      calculator.calculator_frame.currencyConversionResult->text();
    CHECK(initial_conversion == "0");

    AND_GIVEN("There is an active internet connection")
    {
      const cpr::Response resp =
        cpr::Get(cpr::Url{ "https://clients3.google.com/generate_204" },
                 cpr::Timeout{ 3000 });

      REQUIRE(resp.error.code == cpr::ErrorCode::OK);
      REQUIRE(resp.status_code == 204);
    }

    WHEN("The base currency amount is set to a value other than 0")
    {
      calculator.calculator_frame.fromCurrencySpinBox->setValue(1.0);

      THEN("The conversion rate result is greater than 0")
      {
        QString conversion =
          calculator.calculator_frame.currencyConversionResult->text();
        CHECK(conversion.toFloat() > 0.0);
      }
    }
  }
}