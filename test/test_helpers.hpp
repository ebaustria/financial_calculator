#ifndef FINANCIAL_CALCULATOR_TEST_HELPERS_HPP
#define FINANCIAL_CALCULATOR_TEST_HELPERS_HPP
#include <cmath>

inline bool
effectively_equal(const double a, const double b, const double epsilon = 1e-12f)
{
  const double res = std::fabs(a - b);
  return res < epsilon;
}

#endif // FINANCIAL_CALCULATOR_TEST_HELPERS_HPP
