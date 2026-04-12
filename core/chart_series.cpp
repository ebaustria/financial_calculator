#include "chart_series.hpp"

#include <cmath>

double
CompoundingInterestStrategy::calculate(const uint8_t year)
{
  const double int_rate_converted = interest_rate * 0.01;
  return principal * std::pow(1 + int_rate_converted / compound_rate, year);
}

double
SimpleInterestStrategy::calculate(const uint8_t year)
{
  const double int_rate_converted = interest_rate * 0.01;
  return principal * (1 + int_rate_converted * year);
}

ChartSeries::ChartSeries()
{
  line_series = new QLineSeries{};
}

ChartSeries::~ChartSeries()
{
  delete line_series;
}

void
ChartSeries::set_strategy(Strategy* strat)
{
  strategy = strat;
  max_x = strategy->years;
}

void
ChartSeries::fill_series()
{
  QList<QPointF> new_points;
  for (uint8_t year = 0; year <= strategy->years; ++year) {
    const double final_value = strategy->calculate(year);
    new_points.append(QPointF{ static_cast<double>(year), final_value });
    if (year == 0) {
      min_y = final_value;
    }
    if (year == strategy->years) {
      max_y = final_value;
    }
  }
  line_series->replace(new_points);
}
