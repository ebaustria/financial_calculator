#ifndef FINANCIAL_CALCULATOR_CHART_SERIES_HPP
#define FINANCIAL_CALCULATOR_CHART_SERIES_HPP
#include <QtCharts/QLineSeries>

class Strategy
{
public:
  explicit Strategy(const uint8_t t_years)
    : years{ t_years }
  {
  }
  virtual ~Strategy() = default;
  virtual double calculate(uint8_t year) = 0;
  uint8_t years;
};

class CompoundingInterestStrategy : public Strategy
{
public:
  CompoundingInterestStrategy(const float prin,
                              const float int_rate,
                              const float comp_rate,
                              const uint8_t t_years)
    : Strategy(t_years)
    , principal{ prin }
    , interest_rate{ int_rate }
    , compound_rate{ comp_rate }
  {
  }

  double calculate(uint8_t year) override;

private:
  float principal;
  float interest_rate;
  float compound_rate;
};

class SimpleInterestStrategy : public Strategy
{
public:
  SimpleInterestStrategy(const float prin,
                         const float int_rate,
                         const uint8_t t_years)
    : Strategy(t_years)
    , principal{ prin }
    , interest_rate{ int_rate }
  {
  }

  double calculate(uint8_t year) override;

private:
  float principal;
  float interest_rate;
};

class ChartSeries
{
public:
  ChartSeries();
  ~ChartSeries();
  void set_strategy(Strategy* strat);
  void fill_series();

  QLineSeries* line_series{ nullptr };
  uint8_t min_x{ 0 };
  uint8_t max_x{ 0 };
  double min_y{ 0 };
  double max_y{ 0 };

private:
  Strategy* strategy{ nullptr };
};

#endif // FINANCIAL_CALCULATOR_CHART_SERIES_HPP
