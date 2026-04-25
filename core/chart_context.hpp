#ifndef FINANCIAL_CALCULATOR_CHART_CONTEXT_HPP
#define FINANCIAL_CALCULATOR_CHART_CONTEXT_HPP
#include "strategy.hpp"

class ChartContext
{
public:
  ChartContext();
  void set_strategy(Strategy* strat);
  void replace_series() const;

  QLineSeries* line_series{ nullptr };
  Strategy* strategy{ nullptr };
};

#endif // FINANCIAL_CALCULATOR_CHART_CONTEXT_HPP
