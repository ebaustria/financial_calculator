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
