#include "chart_series.hpp"

#include <cmath>

QList<QPointF>
Strategy::calculate_all()
{
  QList<QPointF> new_points;
  max_x = max_periods;
  for (uint16_t period = 0; period <= max_periods; ++period) {
    const double final_value = calculate(period);
    new_points.append(QPointF{ static_cast<double>(period), final_value });
    if (period == 0) {
      min_y = final_value;
    }
    if (period == max_periods) {
      max_y = final_value;
    }
  }
  return new_points;
}

double
CompoundingInterestStrategy::calculate(const uint16_t period)
{
  const double int_rate_converted = interest_rate * 0.01;
  return principal * std::pow(1 + int_rate_converted / compound_rate, period);
}

double
SimpleInterestStrategy::calculate(const uint16_t period)
{
  const double int_rate_converted = interest_rate * 0.01;
  return principal * (1 + int_rate_converted * period);
}

QList<QPointF>
LoanRepaymentStrategy::calculate_all()
{
  QList<QPointF> new_points;
  if (is_amortizing()) {
    uint16_t period = 0;
    double remaining_balance = calculate(period);
    max_y = remaining_balance;
    new_points.append(
      QPointF{ static_cast<double>(period), remaining_balance });
    while (remaining_balance > 0.0) {
      period++;
      remaining_balance = calculate(period);
      if (remaining_balance < 0.0) {
        remaining_balance = 0.0;
      }
      new_points.append(
        QPointF{ static_cast<double>(period), remaining_balance });
    }
    max_x = period;
    min_y = remaining_balance;
  } else {
    new_points = Strategy::calculate_all();
  }
  return new_points;
}

double
LoanRepaymentStrategy::calculate(const uint16_t period)
{
  const double total_interest = std::pow(1 + monthly_interest, period);
  const double future_value = principal * total_interest;
  const double fv_annuity =
    monthly_payment * ((total_interest - 1) / monthly_interest);
  return future_value - fv_annuity;
}

bool
LoanRepaymentStrategy::is_amortizing() const
{
  return monthly_payment > principal * monthly_interest;
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
}

void
ChartSeries::replace_series() const
{
  const QList<QPointF> new_points = strategy->calculate_all();
  line_series->replace(new_points);
}
