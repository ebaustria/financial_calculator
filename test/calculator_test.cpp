#include "app/calculator.hpp"
#include "ui_calculator_window.h"
#include <catch2/catch_test_macros.hpp>
#include <cpr/cpr.h>

#include "test_fixture.hpp"

int argc = 0;
char* argv[] = {};
QApplication app{ argc, argv };

SCENARIO("The user can convert from one currency to another by changing the "
         "base currency amount")
{
  GIVEN("The calculator app is initialized")
  {
    const TestFixture test_fixture;
    QString initial_conversion = test_fixture.calculator->calculator_frame
                                   .currencyConversionResult->text();
    CHECK(initial_conversion == "0");

    AND_GIVEN("There is an active internet connection")
    {
      TestFixture::check_internet();
    }

    WHEN("The base currency amount is set to a value other than 0")
    {
      test_fixture.calculator->calculator_frame.fromCurrencySpinBox->setValue(
        1.0);

      THEN("The conversion rate result is greater than 0")
      {
        const QString conversion = test_fixture.calculator->calculator_frame
                                     .currencyConversionResult->text();
        CHECK(conversion.toFloat() > 0.0);
      }
    }
  }
}

SCENARIO("The user can convert from one currency to another by swapping the "
         "base and target currencies")
{
  GIVEN("The calculator app is initialized and there is an active internet "
        "connection")
  {
    const TestFixture test_fixture;
    TestFixture::check_internet();
    CHECK(test_fixture.calculator->calculator_frame.fromCurrencyComboBox
            ->currentIndex() == 0);
    CHECK(test_fixture.calculator->calculator_frame.toCurrencyComboBox
            ->currentIndex() == 1);

    WHEN("The base currency amount is set to a value other than 0 and the base "
         "and target currencies are swapped")
    {
      test_fixture.calculator->calculator_frame.fromCurrencySpinBox->setValue(
        1.0);
      const float initial_conversion =
        test_fixture.get_current_conversion_result();
      test_fixture.calculator->calculator_frame.fromCurrencyComboBox
        ->setCurrentIndex(1);

      THEN("The conversion rate is recalculated and the combo box indices are "
           "swapped")
      {
        const float final_conversion =
          test_fixture.get_current_conversion_result();
        TestFixture::check_conversion_recalculation(initial_conversion,
                                                    final_conversion);
        CHECK(test_fixture.calculator->calculator_frame.fromCurrencyComboBox
                ->currentIndex() == 1);
        CHECK(test_fixture.calculator->calculator_frame.toCurrencyComboBox
                ->currentIndex() == 0);
      }
    }
  }
}

SCENARIO("The conversion rate is updated whenever the base or target "
         "currencies are changed")
{
  GIVEN("The calculator app is initialized and there is an active internet "
        "connection")
  {
    const TestFixture test_fixture;
    TestFixture::check_internet();

    WHEN("The base currency is changed and then the target currency is changed")
    {
      test_fixture.calculator->calculator_frame.fromCurrencyComboBox
        ->setCurrentIndex(3);
      test_fixture.calculator->calculator_frame.fromCurrencySpinBox->setValue(
        1.0);
      const float first_conversion =
        test_fixture.get_current_conversion_result();
      test_fixture.calculator->calculator_frame.toCurrencyComboBox
        ->setCurrentIndex(2);
      const float second_conversion =
        test_fixture.get_current_conversion_result();

      THEN("The conversion rate is recalculated")
      {
        TestFixture::check_conversion_recalculation(first_conversion,
                                                    second_conversion);
      }
    }
  }
}