#include "test_fixture.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cpr/cpr.h>

TestFixture::TestFixture()
{
  calculator = new Calculator{ &window };
  fin_calc_window.setupUi(&window);
  window.setCentralWidget(calculator);
}

void
TestFixture::check_internet()
{
  const cpr::Response resp =
    cpr::Get(cpr::Url{ "https://clients3.google.com/generate_204" },
             cpr::Timeout{ 3000 });

  REQUIRE(resp.error.code == cpr::ErrorCode::OK);
  REQUIRE(resp.status_code == 204);
}

void
TestFixture::check_conversion_recalculation(const float first_conversion,
                                            const float second_conversion)
{
  if (first_conversion == 1.0) {
    CHECK(first_conversion == second_conversion);
    return;
  }
  CHECK(first_conversion != second_conversion);
}

float
TestFixture::get_current_conversion_result() const
{
  return calculator->calculator_frame.currencyConversionResult->text()
    .toFloat();
}