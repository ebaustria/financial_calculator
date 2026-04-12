#include "chart_series.hpp"

#include <cmath>

double
CompoundingInterestStrategy::calculate(const uint8_t year)
{
  const double int_rate_converted = interest_rate * 0.01;
  return principal * std::pow(1 + int_rate_converted / compound_rate, year);
}

ChartSeries::ChartSeries()
{
  line_series = new QLineSeries();
}

ChartSeries::~ChartSeries()
{
  delete line_series;
}

void
ChartSeries::set_strategy(Strategy* strat)
{
  strategy = strat;
}

void
ChartSeries::fill_series()
{
  for (uint8_t year = 0; year <= strategy->years; ++year) {
    const double final_value = strategy->calculate(year);
    line_series->append(year, final_value);
  }
}