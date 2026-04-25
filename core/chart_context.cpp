#include "chart_context.hpp"

ChartContext::ChartContext()
{
  line_series = new QLineSeries{};
}

void
ChartContext::set_strategy(Strategy* strat)
{
  strategy = strat;
}

void
ChartContext::replace_series() const
{
  const QList<QPointF> new_points = strategy->calculate_all();
  line_series->replace(new_points);
}